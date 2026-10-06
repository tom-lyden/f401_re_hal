//
// Created by tomly on 05/10/2026.
//

#include <i2c_fsm.h>
#include <i2c_fsm_ctrl_write.h>

#include <i2c_internal.h>
#include <mmio.h>

#define EXTRACT_2_MSB_FROM_10BIT_ADDRESS(address) (((address) >> 8U) & 0x3U))
#define EXTRACT_7BIT_ADDRESS(address) ((address) & 0x7F)
#define EXTRACT_8BIT_ADDRESS(address) ((address) & 0xFF)
#define READ_BIT(val, bit) (!!(val & (1U << bit)))

static void i2c_fsm_idle_enter(i2c_callback_ctx_t* ctx);
static void i2c_fsm_idle_update(i2c_callback_ctx_t* ctx);
static void i2c_fsm_idle_handle_evt(i2c_callback_ctx_t* ctx);
static void i2c_fsm_start_enter(i2c_callback_ctx_t* ctx);
static void i2c_fsm_start_update(i2c_callback_ctx_t* ctx);
static void i2c_fsm_start_handle_evt(i2c_callback_ctx_t* ctx);
static void i2c_fsm_addr_enter(i2c_callback_ctx_t* ctx);
static void i2c_fsm_addr_update(i2c_callback_ctx_t* ctx);
static void i2c_fsm_addr_handle_evt(i2c_callback_ctx_t* ctx);
static void i2c_fsm_data_enter(i2c_callback_ctx_t* ctx);
static void i2c_fsm_data_update(i2c_callback_ctx_t* ctx);
static void i2c_fsm_data_handle_evt(i2c_callback_ctx_t* ctx);
static void i2c_fsm_stop_enter(i2c_callback_ctx_t* ctx);
static void i2c_fsm_stop_update(i2c_callback_ctx_t* ctx);
static void i2c_fsm_stop_handle_evt(i2c_callback_ctx_t* ctx);
static void i2c_fsm_done_enter(i2c_callback_ctx_t* ctx);
static void i2c_fsm_done_update(i2c_callback_ctx_t* ctx);
static void i2c_fsm_done_handle_evt(i2c_callback_ctx_t* ctx);
static void i2c_fsm_err_enter(i2c_callback_ctx_t* ctx);
static void i2c_fsm_err_update(i2c_callback_ctx_t* ctx);
static void i2c_fsm_err_handle_evt(i2c_callback_ctx_t* ctx);

static uint8_t get_first_address_byte(i2c_device_address_t address);

static const i2c_fsm_state_callbacks_t write_callbacks[I2C_FSM_NUM_STATES] =
{
	[I2C_FSM_STATE_IDLE] = 
	{
		.enter = i2c_fsm_idle_enter,
		.handle_evt = i2c_fsm_idle_handle_evt,
		.update = i2c_fsm_idle_update,
	},
	[I2C_FSM_STATE_START] = 
	{
		.enter = i2c_fsm_start_enter,
		.handle_evt = i2c_fsm_start_handle_evt,
		.update = i2c_fsm_start_update,
	},
	[I2C_FSM_STATE_ADDR] = 
	{
		.enter = i2c_fsm_addr_enter,
		.handle_evt = i2c_fsm_addr_handle_evt,
		.update = i2c_fsm_addr_update,
	},
	[I2C_FSM_STATE_DATA] = 
	{
		.enter = i2c_fsm_data_enter,
		.handle_evt = i2c_fsm_data_handle_evt,
		.update = i2c_fsm_data_update,
	},
	[I2C_FSM_STATE_STOP] = 
	{
		.enter = i2c_fsm_stop_enter,
		.handle_evt = i2c_fsm_stop_handle_evt,
		.update = i2c_fsm_stop_update,
	},
	[I2C_FSM_STATE_DONE] = 
	{
		.enter = i2c_fsm_done_enter,
		.handle_evt = i2c_fsm_done_handle_evt,
		.update = i2c_fsm_done_update,
	},
	[I2C_FSM_STATE_ERROR] = 
	{
		.enter = i2c_fsm_err_enter,
		.handle_evt = i2c_fsm_err_handle_evt,
		.update = i2c_fsm_err_update,
	},
};

const i2c_fsm_state_callbacks_t* I2C_FSM_GetCtrlWriteCallbacks(void)
{
	return write_callbacks;
}

static void i2c_fsm_idle_enter(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_idle_handle_evt(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_idle_update(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_start_enter(i2c_callback_ctx_t* ctx)
{
	volatile i2c_t* i2c = I2C_PORT_ADDR(ctx->bus);
	
	MMIO_WriteField(&i2c->CR1, I2C_CR1_START_FIELD, I2C_CR1_START_WIDTH, 1);
}

static void i2c_fsm_start_handle_evt(i2c_callback_ctx_t* ctx)
{
	I2C_FSM_SetState(ctx->fsm, I2C_FSM_STATE_ADDR);
}

static void i2c_fsm_start_update(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_addr_enter(i2c_callback_ctx_t* ctx)
{
	uint8_t byte = get_first_address_byte(ctx->req->device_address);
	
	volatile i2c_t* i2c = I2C_PORT_ADDR(ctx->bus);
	
	(void)i2c->SR1; // Clear SB to prevent further START generations
	i2c->DR = byte;
}

static void i2c_fsm_addr_handle_evt(i2c_callback_ctx_t* ctx)
{
	volatile i2c_t* i2c = I2C_PORT_ADDR(ctx->bus);
	uint32_t sr1 = i2c->SR1;
	
	if (READ_BIT(sr1, I2C_SR1_ADD10_FIELD))
	{
		uint8_t byte = EXTRACT_8BIT_ADDRESS(ctx->req->device_address.address);
		i2c->DR = byte;
	}
	else if (READ_BIT(sr1, I2C_SR1_ADDR_FIELD))
	{
		(void)i2c->SR2; // Clear ADDR to allow sending bytes
		I2C_FSM_SetState(ctx->fsm, I2C_FSM_STATE_DATA);
	}
}

static void i2c_fsm_addr_update(i2c_callback_ctx_t* ctx)
{
	
}

static void write_next_byte(i2c_callback_ctx_t* ctx)
{
	const uint8_t* data;
	uint32_t idx;
	
	if (ctx->byte_counter < ctx->req->register_addr_length)
	{
		data = ctx->req->register_addr;
		idx = ctx->byte_counter;
	}
	else
	{
		data = ctx->req->data_buffer;
		idx = ctx->byte_counter - ctx->req->register_addr_length;
	}
	
	uint8_t byte = data[idx];
	
	volatile i2c_t* i2c = I2C_PORT_ADDR(ctx->bus);
	i2c->DR = byte;
	ctx->byte_counter++;
}

static void i2c_fsm_data_enter(i2c_callback_ctx_t* ctx)
{
	write_next_byte(ctx);
}

static void i2c_fsm_data_handle_evt(i2c_callback_ctx_t* ctx)
{
	volatile i2c_t* i2c = I2C_PORT_ADDR(ctx->bus);
	uint32_t sr1 = i2c->SR1;
	
	uint32_t total_bytes = ctx->req->register_addr_length + ctx->req->data_length;
	
	if (READ_BIT(sr1, I2C_SR1_TxE_FIELD) && ctx->byte_counter < total_bytes)
		write_next_byte(ctx);
	else if (READ_BIT(sr1, I2C_SR1_BTF_FIELD) && ctx->byte_counter >= total_bytes)
		I2C_FSM_SetState(ctx->fsm, I2C_FSM_STATE_STOP);
}

static void i2c_fsm_data_update(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_stop_enter(i2c_callback_ctx_t* ctx)
{
	volatile i2c_t* i2c = I2C_PORT_ADDR(ctx->bus);
	MMIO_WriteField(&i2c->CR1, I2C_CR1_STOP_FIELD, I2C_CR1_STOP_WIDTH, 1);
}

static void i2c_fsm_stop_handle_evt(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_stop_update(i2c_callback_ctx_t* ctx)
{
	volatile i2c_t* i2c = I2C_PORT_ADDR(ctx->bus);
	if (MMIO_ReadField(&i2c->CR1, I2C_CR1_STOP_FIELD, I2C_CR1_STOP_WIDTH))
		return;
	
	I2C_FSM_SetState(ctx->fsm, I2C_FSM_STATE_DONE);
}

static void i2c_fsm_done_enter(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_done_handle_evt(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_done_update(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_err_enter(i2c_callback_ctx_t* ctx)
{
	volatile i2c_t* i2c = I2C_PORT_ADDR(ctx->bus);
	uint32_t sr1 = i2c->SR1;
	
	if (READ_BIT(sr1, I2C_SR1_BERR_FIELD))
	{
		MMIO_WriteField(&i2c->SR1, I2C_SR1_BERR_FIELD, I2C_SR1_BERR_WIDTH, 0);
	}
	if (READ_BIT(sr1, I2C_SR1_ARLO_FIELD))
	{
		MMIO_WriteField(&i2c->SR1, I2C_SR1_ARLO_FIELD, I2C_SR1_ARLO_WIDTH, 0);
	}
	if (READ_BIT(sr1, I2C_SR1_AF_FIELD))
	{
		MMIO_WriteField(&i2c->SR1, I2C_SR1_AF_FIELD, I2C_SR1_AF_WIDTH, 0);
		MMIO_WriteField(&i2c->CR1, I2C_CR1_STOP_FIELD, I2C_CR1_STOP_WIDTH, 1);
	}
	if (READ_BIT(sr1, I2C_SR1_OVR_FIELD))
	{
		MMIO_WriteField(&i2c->SR1, I2C_SR1_OVR_FIELD, I2C_SR1_OVR_WIDTH, 0);
		ctx->byte_counter--;
		I2C_FSM_SetState(ctx->fsm, I2C_FSM_STATE_DATA);
	}
}

static void i2c_fsm_err_handle_evt(i2c_callback_ctx_t* ctx)
{
	
}

static void i2c_fsm_err_update(i2c_callback_ctx_t* ctx)
{
	
}

static uint8_t get_first_address_byte(i2c_device_address_t address)
{
	switch (address.type)
	{
		case I2C_DEVICE_ADDRESS_10BIT:
			return (uint8_t)(I2C_DEVICE_ADDRESS_10BIT_HEADER | (EXTRACT_2_MSB_FROM_10BIT_ADDRESS(address.address) << 1U);
			break;
		case I2C_DEVICE_ADDRESS_7BIT:
		return (uint8_t)(EXTRACT_7BIT_ADDRESS(address.address) << 1U);
		break;
	default:
	__builtin_unreachable();
}
}
