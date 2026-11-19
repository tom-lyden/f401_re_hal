//
// Created by tomly on 26/09/2026.
//

#ifndef F401_RE_HAL_I2C_INTERNAL_H
#define F401_RE_HAL_I2C_INTERNAL_H

#include <i2c.h>
#include <rcc.h>
#include <gpio.h>

#include <stdint.h>

#define I2C_BASE_ADDR (0x40005400U)
#define I2C_PORT_WIDTH (0x0400U)

#define I2C_PORT_ADDR(port) ((volatile i2c_t *)((I2C_BASE_ADDR) + ((port) * (I2C_PORT_WIDTH))))

#define I2C_CR1_EN_OFFSET (0U)
#define I2C_CR1_START_OFFSET (8U)
#define I2C_CR1_STOP_OFFSET (9U)

#define I2C_CR2_FREQ_OFFSET (0U)
#define I2C_CR2_FREQ_WIDTH (6U)
#define I2C_CR2_ITERREN_OFFSET (8U)
#define I2C_CR2_ITEVTEN_OFFSET (9U)
#define I2C_CR2_ITBUFEN_OFFSET (10U)

#define I2C_SR1_ADDR_OFFSET (1U)
#define I2C_SR1_BTF_OFFSET (2U)
#define I2C_SR1_ADD10_OFFSET (3U)
#define I2C_SR1_TxE_OFFSET (7U)
#define I2C_SR1_BERR_OFFSET (8U)
#define I2C_SR1_ARLO_OFFSET (9U)
#define I2C_SR1_AF_OFFSET (10U)
#define I2C_SR1_OVR_OFFSET (11U)

#define I2C_CCR_MODE_OFFSET (15U)
#define I2C_CCR_DUTY_OFFSET (14U)
#define I2C_CCR_VALUE_WIDTH (12U)
#define I2C_CCR_WIDTH (16U)

#define I2C_STANDARD_MODE_MIN_PCLK1_FREQ_MHZ (2)
#define I2C_FAST_MODE_MIN_PCLK1_FREQ_MHZ (4)

#define I2C_MIN_PCLK1_FREQ_MHZ (2)
#define I2C_MAX_PCLK1_FREQ_MHZ (50)

#define I2C_CLK_MODE_STANDARD_CCR_DENOM_CONST (2)
#define I2C_CLK_MODE_FAST_DC_2_CCR_DENOM_CONST (3)
#define I2C_CLK_MODE_FAST_DC_16_9_CCR_DENOM_CONST (25)

#define I2C_STANDARD_MODE_MIN_CCR (4)
#define I2C_FAST_MODE_MIN_CCR (1)

#define I2C_TRISE_WIDTH (6U)

#define I2C_MAX_ADDRESS_VALUE_7BIT (0x7FU)
#define I2C_MAX_ADDRESS_VALUE_10BIT (0x3FFU)
#define I2C_DEVICE_ADDRESS_10BIT_HEADER (0b11110000)
#define I2C_DEVICE_ADDRESS_10BIT_NUM_BITS (10)
#define I2C_DEVICE_ADDRESS_10BIT_HEADER_MSB_BITS (2)

typedef struct
{
	uint32_t CR1;
	uint32_t CR2;
	uint32_t OAR1;
	uint32_t OAR2;
	uint32_t DR;
	uint32_t SR1;
	uint32_t SR2;
	uint32_t CCR;
	uint32_t TRISE;
	uint32_t FLTR;
} i2c_t;

typedef struct
{
	i2c_bus_t bus;
	gpio_pin_t sda_pin;
	gpio_pin_t scl_pin;
	gpio_port_t sda_port;
	gpio_port_t scl_port;
	gpio_af_t sda_af;
	gpio_af_t scl_af;
} i2c_gpio_cfg_t;

i2c_gpio_cfg_t I2C_BuildGPIOConfig(i2c_sda_pin_t sda, i2c_scl_pin_t scl);
rcc_en_i2c_port_t I2C_GetRCCPort(i2c_bus_t bus);

#endif // F401_RE_HAL_I2C_INTERNAL_H
