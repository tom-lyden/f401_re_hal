//
// Created by tomly on 29/09/2026.
//

#include <i2c_internal.h>
#include <i2c.h>
#include <gpio.h>
#include <rcc.h>

#include <assert.h>

typedef struct
{
	i2c_bus_t bus;
	gpio_pin_t pin;
	gpio_port_t port;
	gpio_af_t af;
} i2c_pin_data_t;

static const i2c_pin_data_t sda_pin_data[I2C_SDA_NUM_PINS] =
{
	[I2C1_SDA_PB7] =
	{
		.bus  = I2C_BUS_1,
		.pin  = GPIO_PIN_7,
		.port = GPIO_PORT_B,
		.af   = GPIO_AF_4,
	},
	[I2C1_SDA_PB9] =
	{
		.bus  = I2C_BUS_1,
		.pin  = GPIO_PIN_9,
		.port = GPIO_PORT_B,
		.af   = GPIO_AF_4,
	},
	[I2C2_SDA_PB3] =
	{
		.bus  = I2C_BUS_2,
		.pin  = GPIO_PIN_3,
		.port = GPIO_PORT_B,
		.af   = GPIO_AF_9,
	},
	[I2C2_SDA_PB11] =
	{
		.bus  = I2C_BUS_2,
		.pin  = GPIO_PIN_11,
		.port = GPIO_PORT_B,
		.af   = GPIO_AF_4,
	},
	[I2C3_SDA_PB4] =
	{
		.bus  = I2C_BUS_3,
		.pin  = GPIO_PIN_4,
		.port = GPIO_PORT_B,
		.af   = GPIO_AF_9,
	},
	[I2C3_SDA_PC9] =
	{
		.bus  = I2C_BUS_3,
		.pin  = GPIO_PIN_9,
		.port = GPIO_PORT_C,
		.af   = GPIO_AF_4,
	},
};

static const i2c_pin_data_t scl_pin_data[I2C_SCL_NUM_PINS] =
{
	[I2C1_SCL_PB6] =
	{
		.bus  = I2C_BUS_1,
		.pin  = GPIO_PIN_6,
		.port = GPIO_PORT_B,
		.af   = GPIO_AF_4,
	},
	[I2C1_SCL_PB8] =
	{
		.bus  = I2C_BUS_1,
		.pin  = GPIO_PIN_8,
		.port = GPIO_PORT_B,
		.af   = GPIO_AF_4,
	},
	[I2C2_SCL_PB10] =
	{
		.bus  = I2C_BUS_2,
		.pin  = GPIO_PIN_10,
		.port = GPIO_PORT_B,
		.af   = GPIO_AF_4,
	},
	[I2C3_SCL_PA8] =
	{
		.bus  = I2C_BUS_3,
		.pin  = GPIO_PIN_8,
		.port = GPIO_PORT_A,
		.af   = GPIO_AF_4,
	},
};

i2c_gpio_cfg_t I2C_Build_GPIO_Config(i2c_sda_pin_t sda, i2c_scl_pin_t scl)
{
	assert(sda < I2C_SDA_NUM_PINS && scl < I2C_SCL_NUM_PINS);
	
	i2c_pin_data_t sda_data = sda_pin_data[sda];
	i2c_pin_data_t scl_data = scl_pin_data[scl];

	assert(sda_data.bus == scl_data.bus);

	i2c_gpio_cfg_t result =
	{
		.bus      = sda_data.bus,
		.sda_pin  = sda_data.pin,
		.scl_pin  = scl_data.pin,
		.sda_port = sda_data.port,
		.scl_port = scl_data.port,
		.sda_af   = sda_data.af,
		.scl_af   = scl_data.af,
	};

	return result;
}

rcc_en_i2c_port_t I2C_RCC_EN_By_I2C(i2c_bus_t bus)
{
	switch (bus)
	{
		case I2C_BUS_1:
			return RCC_EN_I2C1;
		case I2C_BUS_2:
			return RCC_EN_I2C2;
		case I2C_BUS_3:
			return RCC_EN_I2C3;
		default:
			__builtin_unreachable();
	}
}
