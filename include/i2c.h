//
// Created by tomly on 24/09/2026.
//

#ifndef F401_RE_HAL_I2C_H
#define F401_RE_HAL_I2C_H

#include <stdbool.h>
#include <stdint.h>

#define CLK_MODE_STANDARD_FREQ_HZ (100000)
#define CLK_MODE_FAST_FREQ_HZ (400000)

#define CLK_MODE_STANDARD_TRISE_NS (1000)
#define CLK_MODE_FAST_TRISE_NS (300)

typedef enum
{
	I2C_BUS_1 = 0,
	I2C_BUS_2,
	I2C_BUS_3,
	NUM_I2C_BUS_TYPES
} i2c_bus_t;

typedef enum
{
	I2C_CLK_MODE_STANDARD = 0,
	I2C_CLK_MODE_FAST_DC_2,
	I2C_CLK_MODE_FAST_DC_16_9,
	NUM_I2C_CLOCK_MODES
} i2c_clk_mode_t;

typedef enum
{
	I2C1_SDA_PB7 = 0,
	I2C1_SDA_PB9,
	I2C2_SDA_PB3,
	I2C2_SDA_PB11,
	I2C3_SDA_PB4,
	I2C3_SDA_PC9,
	I2C_SDA_NUM_PINS
} i2c_sda_pin_t;

typedef enum
{
	I2C1_SCL_PB6 = 0,
	I2C1_SCL_PB8,
	I2C2_SCL_PB10,
	I2C3_SCL_PA8,
	I2C_SCL_NUM_PINS
} i2c_scl_pin_t;

typedef enum
{
	I2C_BUS_REQ_QUEUED = 0,
	I2C_BUS_REQ_IN_PROGRESS,
	I2C_BUS_REQ_DONE,
	I2C_BUS_REQ_ERROR,
} i2c_req_state_t;

typedef enum
{
	I2C_DEVICE_ADDRESS_7BIT  = 7,
	I2C_DEVICE_ADDRESS_10BIT = 10,
} i2c_device_address_type_t;

typedef struct
{
	i2c_sda_pin_t sda_pin;
	i2c_scl_pin_t scl_pin;
	i2c_clk_mode_t clk_mode;
	uint32_t clk_freq_hz;
} i2c_cfg_t;

typedef struct
{
	i2c_device_address_type_t type;
	uint16_t address;
} i2c_device_address_t;

typedef struct
{
	i2c_device_address_t device_address;
	const uint8_t* register_addr;
	uint32_t register_addr_length;
	const uint8_t* data_buffer;
	uint32_t data_length;
	volatile i2c_req_state_t state; // For reading only; updated by I2C driver
} i2c_req_t;

void I2C_Init(const i2c_cfg_t* cfg);
bool I2C_Write(i2c_bus_t bus, i2c_req_t* req);

void I2C_Update(void);

#endif // F401_RE_HAL_I2C_H
