//
// Created by tomly on 24/09/2026.
//

#include <i2c.h>
#include <i2c_internal.h>
#include <gpio.h>
#include <mmio.h>
#include <rcc.h>
#include <assert.h>

#define HZ_TO_MHZ(hz) ((hz) / 1000000UL)
#define NSEC_PER_SEC (1000000000UL)
#define MAX_VALUE_IN_WIDTH(width) ((1U << (width)) - 1)

static uint8_t calculate_freq(uint32_t pclk1);
static uint16_t calculate_ccr(i2c_clk_mode_t clk_mode, uint32_t clk_freq_hz, uint32_t pclk1);
static uint8_t calculate_trise(i2c_clk_mode_t clk_mode, uint32_t pclk1);
static void update_i2c_bus(i2c_bus_t bus);

static bool is_bus_initialized[NUM_I2C_BUS_TYPES] = { false };

void I2C_Init(const i2c_cfg_t* cfg)
{
	assert(cfg->clk_freq_hz > 0);
	
	if (cfg->clk_mode == I2C_CLK_MODE_STANDARD)
		assert(cfg->clk_freq_hz <= CLK_MODE_STANDARD_FREQ_HZ);
	else
		assert(cfg->clk_freq_hz <= CLK_MODE_FAST_FREQ_HZ);
	
	i2c_gpio_cfg_t i2c_gpio_cfg = I2C_Build_GPIO_Config(cfg->sda_pin, cfg->scl_pin);
	
	assert(!is_bus_initialized[i2c_gpio_cfg.bus]);
	
	rcc_en_i2c_port_t rcc_en = I2C_RCC_EN_By_I2C(i2c_gpio_cfg.bus);
	RCC_EnableI2C(rcc_en);
	
	volatile i2c_t* i2c = I2C_PORT_ADDR(i2c_gpio_cfg.bus);
	
	MMIO_WriteField(&i2c->CR1, I2C_CR1_EN_FIELD, I2C_CR1_EN_WIDTH, 0);
	
	uint32_t pclk1 = RCC_GetPCLK1();
	uint8_t freq = calculate_freq(pclk1);
	
	if (cfg->clk_mode == I2C_CLK_MODE_STANDARD)
	{
		assert(freq >= I2C_STANDARD_MODE_MIN_PCLK1_FREQ_MHZ); 
	}
	else
	{
		assert(freq >= I2C_FAST_MODE_MIN_PCLK1_FREQ_MHZ);
	}
	
	MMIO_WriteField(&i2c->CR2, I2C_CR2_FREQ_FIELD, I2C_CR2_FREQ_WIDTH, freq);
	
	i2c->CCR = calculate_ccr(cfg->clk_mode, cfg->clk_freq_hz, pclk1) | 
		((cfg->clk_mode != I2C_CLK_MODE_STANDARD) << I2C_CCR_MODE_FIELD) | 
		((cfg->clk_mode == I2C_CLK_MODE_FAST_DC_16_9) << I2C_CCR_DUTY_FIELD);
	
	i2c->TRISE = calculate_trise(cfg->clk_mode, pclk1);
	
	gpio_config_t scl_config = 
	{
		.mode = GPIO_MODE_ALTERNATE,
		.type = GPIO_TYPE_OPEN_DRAIN,
		.ospeed = GPIO_SPEED_LOW,
		.pupd = GPIO_PUPD_NONE,
		.af = i2c_gpio_cfg.scl_af
	};
	
	GPIO_Init(i2c_gpio_cfg.scl_port, i2c_gpio_cfg.scl_pin, &scl_config);
	
	gpio_config_t sda_config = 
	{
		.mode = GPIO_MODE_ALTERNATE,
		.type = GPIO_TYPE_OPEN_DRAIN,
		.ospeed = GPIO_SPEED_LOW,
		.pupd = GPIO_PUPD_NONE,
		.af = i2c_gpio_cfg.sda_af
	};
	
	GPIO_Init(i2c_gpio_cfg.sda_port, i2c_gpio_cfg.sda_pin, &sda_config);
	
	is_bus_initialized[i2c_gpio_cfg.bus] = true;
	MMIO_WriteField(&i2c->CR1, I2C_CR1_EN_FIELD, I2C_CR1_EN_WIDTH, 1);
}

bool I2C_Write(i2c_bus_t bus, i2c_req_t* req)
{
	return true;
}

bool I2C_WriteRegister(i2c_bus_t bus, i2c_req_t* req)
{
	return true;
}

void I2C_Update(void)
{
	for (int i = 0; i < NUM_I2C_BUS_TYPES; i++)
	{
		if (!is_bus_initialized[i])
			continue;
		
		update_i2c_bus(i);
	}
}

static void update_i2c_bus(i2c_bus_t bus)
{
	// read state flags and do stuff
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
