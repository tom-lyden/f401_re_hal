//
// Created by tomly on 24/09/2026.
//

#include <i2c.h>
#include <i2c_internal.h>
#include <i2c_queue.h>
#include <i2c_fsm.h>
#include <i2c_fsm_ctrl_write.h>
#include <gpio.h>
#include <mmio.h>
#include <rcc.h>
#include <nvic.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#define HZ_TO_MHZ(hz) ((hz) / 1000000UL)
#define NSEC_PER_SEC (1000000000UL)
#define MAX_VALUE_IN_WIDTH(width) ((1U << (width)) - 1)

typedef enum
{
	I2C_IRQ_TYPE_EV = 0,
	I2C_IRQ_TYPE_ER,
	NUM_I2C_IRQ_TYPES
} i2c_irq_type_t;

typedef struct
{
	i2c_req_t* req;
	i2c_fsm_t fsm;
	i2c_queue_t queue;
	bool is_initialized;
} i2c_bus_ctrl_t;

static const irq_t irqs_by_bus[NUM_I2C_BUS_TYPES][NUM_I2C_IRQ_TYPES] =
{
	[I2C_BUS_1] = { IRQ_I2C1_EV, IRQ_I2C1_ER },	
	[I2C_BUS_2] = { IRQ_I2C2_EV, IRQ_I2C2_ER },	
	[I2C_BUS_3] = { IRQ_I2C3_EV, IRQ_I2C3_ER },	
};

static i2c_bus_ctrl_t bus_ctrl[NUM_I2C_BUS_TYPES];

static uint8_t calculate_freq(uint32_t pclk1);
static uint16_t calculate_ccr(i2c_clk_mode_t clk_mode, uint32_t clk_freq_hz, uint32_t pclk1);
static uint8_t calculate_trise(i2c_clk_mode_t clk_mode, uint32_t pclk1);
static void update_i2c_bus(i2c_bus_t bus);
static void handle_event(i2c_bus_t bus);
static void handle_error(i2c_bus_t bus);

static void configure_peripheral(i2c_bus_t bus, const i2c_cfg_t* cfg, uint32_t pclk1, uint8_t freq);

void I2C_Init(const i2c_cfg_t* cfg)
{
	assert(cfg != NULL);
	assert(cfg->clk_freq_hz > 0);
	
	uint32_t pclk1 = RCC_GetPCLK1();
	uint8_t freq = calculate_freq(pclk1);
	
	if (cfg->clk_mode == I2C_CLK_MODE_STANDARD)
	{
		assert(cfg->clk_freq_hz <= CLK_MODE_STANDARD_FREQ_HZ);
		assert(freq >= I2C_STANDARD_MODE_MIN_PCLK1_FREQ_MHZ); 
	}
	else if (cfg->clk_mode == I2C_CLK_MODE_FAST_DC_2 || cfg->clk_mode == I2C_CLK_MODE_FAST_DC_16_9)
	{
		assert(cfg->clk_freq_hz <= CLK_MODE_FAST_FREQ_HZ);
		assert(freq >= I2C_FAST_MODE_MIN_PCLK1_FREQ_MHZ);
	}
	else
	{
		assert(false);
	}
	
	i2c_gpio_cfg_t i2c_gpio_cfg = I2C_Build_GPIO_Config(cfg->sda_pin, cfg->scl_pin);
	
	i2c_bus_t bus = i2c_gpio_cfg.bus;
	
	assert(!bus_ctrl[bus].is_initialized);
	
	rcc_en_i2c_port_t rcc_en = I2C_RCC_EN_By_I2C(bus);
	RCC_EnableI2C(rcc_en);
	
	gpio_config_t gpio_config = 
	{
		.mode = GPIO_MODE_ALTERNATE,
		.type = GPIO_TYPE_OPEN_DRAIN,
		.ospeed = GPIO_SPEED_LOW,
		.pupd = GPIO_PUPD_NONE,
		.af = i2c_gpio_cfg.scl_af
	};
	
	GPIO_Init(i2c_gpio_cfg.scl_port, i2c_gpio_cfg.scl_pin, &gpio_config);
	
	gpio_config.af = i2c_gpio_cfg.sda_af;
	
	GPIO_Init(i2c_gpio_cfg.sda_port, i2c_gpio_cfg.sda_pin, &gpio_config);
	
	I2C_Queue_Init(&bus_ctrl[bus].queue);
	
	configure_peripheral(bus, cfg, pclk1, freq);
	bus_ctrl[bus].is_initialized = true;
	
	volatile i2c_t* i2c = I2C_PORT_ADDR(bus);
	MMIO_WriteField(&i2c->CR1, I2C_CR1_EN_FIELD, I2C_CR1_EN_WIDTH, 1);
	
	NVIC_EnableInterrupt(irqs_by_bus[bus][0]);
	NVIC_EnableInterrupt(irqs_by_bus[bus][1]);
}

bool I2C_Write(i2c_bus_t bus, i2c_req_t* req)
{
	if (!bus_ctrl[bus].is_initialized)
		return false;
	
	if (req->device_address.type == I2C_DEVICE_ADDRESS_7BIT)
		assert(req->device_address.address <= I2C_MAX_ADDRESS_VALUE_7BIT);
	else if (req->device_address.type == I2C_DEVICE_ADDRESS_10BIT)
		assert(req->device_address.address <= I2C_MAX_ADDRESS_VALUE_10BIT);
	else
		assert(false);
	
	i2c_queue_t* queue = &bus_ctrl[bus].queue;
	
	if (!I2C_Queue_Enqueue(queue, req))
		return false;
	
	req->state = I2C_BUS_REQ_QUEUED;
	
	return true;
}

void I2C_Update(void)
{
	for (int i = 0; i < NUM_I2C_BUS_TYPES; i++)
		update_i2c_bus(i);
}

void I2C1_EV_Handler(void)
{
	handle_event(I2C_BUS_1);
}

void I2C1_ER_Handler(void)
{
	handle_error(I2C_BUS_1);
}

void I2C2_EV_Handler(void)
{
	handle_event(I2C_BUS_2);
}

void I2C2_ER_Handler(void)
{
	handle_error(I2C_BUS_2);
}

void I2C3_EV_Handler(void)
{
	handle_event(I2C_BUS_3);
}

void I2C3_ER_Handler(void)
{
	handle_error(I2C_BUS_3);
}

static void configure_peripheral(i2c_bus_t bus, const i2c_cfg_t* cfg, uint32_t pclk1, uint8_t freq)
{
	volatile i2c_t* i2c = I2C_PORT_ADDR(bus);
	
	MMIO_WriteField(&i2c->CR1, I2C_CR1_EN_FIELD, I2C_CR1_EN_WIDTH, 0);
	
	MMIO_WriteField(&i2c->CR2, I2C_CR2_FREQ_FIELD, I2C_CR2_FREQ_WIDTH, freq);
	MMIO_WriteField(&i2c->CR2, I2C_CR2_ITERREN_FIELD, I2C_CR2_ITERREN_WIDTH, 1);
	MMIO_WriteField(&i2c->CR2, I2C_CR2_ITEVTEN_FIELD, I2C_CR2_ITEVTEN_WIDTH, 1);
	MMIO_WriteField(&i2c->CR2, I2C_CR2_ITBUFEN_FIELD, I2C_CR2_ITBUFEN_WIDTH, 1);
	
	i2c->CCR = calculate_ccr(cfg->clk_mode, cfg->clk_freq_hz, pclk1);
	i2c->CCR |= (cfg->clk_mode != I2C_CLK_MODE_STANDARD) << I2C_CCR_MODE_FIELD;
	i2c->CCR |=	(cfg->clk_mode == I2C_CLK_MODE_FAST_DC_16_9) << I2C_CCR_DUTY_FIELD;
	
	i2c->TRISE = calculate_trise(cfg->clk_mode, pclk1);
}

static uint8_t calculate_freq(uint32_t pclk1)
{
	uint32_t result = HZ_TO_MHZ(pclk1);
	
	assert(result >= I2C_MIN_PCLK1_FREQ_MHZ && result <= I2C_MAX_PCLK1_FREQ_MHZ);
	
	return (uint8_t)result;
}

static uint16_t calculate_ccr(i2c_clk_mode_t clk_mode, uint32_t clk_freq_hz, uint32_t pclk1)
{
	/*
	 * The following is a simplification of SCL clock conditions that need to be met per mode and duty cycle:
	 * 
	 * Sm mode or SMBus:
	 * T_high = CCR * T_pclk1
	 * T_low = CCR * T_pclk1
	 * => CCR = f_pclk1 / (2 * f_clk)
	 * 
	 * Fm mode with duty cycle t_low / t_high = 2:
	 * T_high = CCR * T_pclk1
	 * T_low = 2 * CCR * T_pclk1
	 * => CCR = f_pclk1 / (3 * f_clk)
	 * 
	 * Fm mode with duty cycle t_low / t_high = 16/9:
	 * T_high = 9 * CCR * T_pclk1
	 * T_low = 16 * CCR * T_pclk1
	 * => CCR = f_pclk1 / (25 * f_clk)
	 */
	
	static const uint32_t consts[NUM_I2C_CLOCK_MODES] = 
	{
		[I2C_CLK_MODE_STANDARD] = I2C_CLK_MODE_STANDARD_CCR_DENOM_CONST, 
		[I2C_CLK_MODE_FAST_DC_2] = I2C_CLK_MODE_FAST_DC_2_CCR_DENOM_CONST, 
		[I2C_CLK_MODE_FAST_DC_16_9] = I2C_CLK_MODE_FAST_DC_16_9_CCR_DENOM_CONST, 
	};
	
	assert(clk_mode < NUM_I2C_CLOCK_MODES);
	
	uint32_t denominator = consts[clk_mode] * clk_freq_hz;
	uint32_t result = pclk1 / denominator;
	uint32_t remainder = pclk1 % denominator;
	
	result += remainder ? 1 : 0;

	assert(result <= MAX_VALUE_IN_WIDTH(I2C_CCR_WIDTH));
	
	if (clk_mode == I2C_CLK_MODE_STANDARD)
	{
		assert(result >= I2C_STANDARD_MODE_MIN_CCR);
	}
	else
	{
		assert(result >= I2C_FAST_MODE_MIN_CCR);
	}
	
	return (uint16_t)result;
}

static uint8_t calculate_trise(i2c_clk_mode_t clk_mode, uint32_t pclk1)
{	
	static const uint64_t max_scl_rise_ns[NUM_I2C_CLOCK_MODES] = 
	{
		[I2C_CLK_MODE_STANDARD] = CLK_MODE_STANDARD_TRISE_NS, 
		[I2C_CLK_MODE_FAST_DC_2] = CLK_MODE_FAST_TRISE_NS, 
		[I2C_CLK_MODE_FAST_DC_16_9] = CLK_MODE_FAST_TRISE_NS, 
	};
	
	assert(clk_mode < NUM_I2C_CLOCK_MODES);
	
	uint64_t result = (max_scl_rise_ns[clk_mode] * pclk1) / NSEC_PER_SEC + 1;
	
	assert(result <= MAX_VALUE_IN_WIDTH(I2C_TRISE_WIDTH));
	
	return (uint8_t)result;
}

static void update_i2c_bus(i2c_bus_t bus)
{
	if (!bus_ctrl[bus].is_initialized)
		return;
	
	i2c_fsm_state_t fsm_state = bus_ctrl[bus].fsm.state;
	i2c_queue_t* queue = &bus_ctrl[bus].queue;
	i2c_req_t* req;
	
	if (fsm_state == I2C_FSM_STATE_DONE || fsm_state == I2C_FSM_STATE_ERROR)
	{
		bus_ctrl[bus].req->state = fsm_state == I2C_FSM_STATE_ERROR ? I2C_BUS_REQ_ERROR : I2C_BUS_REQ_DONE;
		I2C_Queue_Dequeue(queue, &req);
		bus_ctrl[bus].req = NULL;
		I2C_FSM_SetState(&bus_ctrl[bus].fsm, I2C_FSM_STATE_IDLE);
	}
	
	if (!I2C_Queue_Peek(queue, &req))
		return;
	
	if (req->state == I2C_BUS_REQ_IN_PROGRESS)
	{
		I2C_FSM_Update(&bus_ctrl[bus].fsm);
		return;
	}
	
	req->state = I2C_BUS_REQ_IN_PROGRESS;
	bus_ctrl[bus].req = req;
	
	i2c_callback_ctx_t ctx = 
	{
		.fsm = &bus_ctrl[bus].fsm,
		.bus = bus,
		.req = req,
		.byte_counter = 0
	};
	
	I2C_FSM_Init(&bus_ctrl[bus].fsm, I2C_FSM_GetCtrlWriteCallbacks(), ctx); // until we implement all the others, this is just ctrl write
	I2C_FSM_SetState(&bus_ctrl[bus].fsm, I2C_FSM_STATE_START);
}

static void handle_event(i2c_bus_t bus)
{
	I2C_FSM_Handle_Event(&bus_ctrl[bus].fsm);
}

static void handle_error(i2c_bus_t bus)
{
	I2C_FSM_Handle_Error(&bus_ctrl[bus].fsm);
}
