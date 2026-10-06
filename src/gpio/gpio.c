//
// Created by tomly on 02/09/2026.
//

#include <stdint.h>
#include <stdbool.h>
#include <gpio.h>
#include <gpio_internal.h>
#include <mmio.h>
#include <rcc.h>
#include <exti.h>
#include <syscfg.h>

#define GPIO_AFR_REG(pin) ((pin) / (GPIO_AFR_FIELDS_PER_REG))
#define GPIO_AFR_OFFSET(pin) (((pin) % (GPIO_AFR_FIELDS_PER_REG)) * (GPIO_AFR_WIDTH))
#define GPIO_MODER_OFFSET(pin) ((pin) * (GPIO_MODE_WIDTH))
#define GPIO_SPEEDR_OFFSET(pin) ((pin) * (GPIO_OSPEED_WIDTH))
#define GPIO_PUPDR_OFFSET(pin) ((pin) * (GPIO_PUPD_WIDTH))

static rcc_en_gpio_port_t get_rcc_port(gpio_port_t port);
static exti_line_t get_exti_line(gpio_pin_t pin);
static exti_trigger_type_t get_exti_trigger_type(gpio_interrupt_type_t type);

void GPIO_Init(gpio_port_t port, gpio_pin_t pin, const gpio_config_t* cfg)
{
	rcc_en_gpio_port_t rcc_port = get_rcc_port(port);
	RCC_EnableGPIO(rcc_port);

	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);

	MMIO_WriteBitsRMW(&gpio->MODER, GPIO_MODER_OFFSET(pin), GPIO_MODE_WIDTH, cfg->mode);
	MMIO_WriteBitsRMW(&gpio->TYPER, pin, GPIO_TYPE_WIDTH, cfg->type);
	MMIO_WriteBitsRMW(&gpio->OSPEEDR, GPIO_SPEEDR_OFFSET(pin), GPIO_OSPEED_WIDTH, cfg->ospeed);
	MMIO_WriteBitsRMW(&gpio->PUPDR, GPIO_PUPDR_OFFSET(pin), GPIO_PUPD_WIDTH, cfg->pupd);
	MMIO_WriteBitsRMW(&gpio->AFR[GPIO_AFR_REG(pin)], GPIO_AFR_OFFSET(pin), GPIO_AFR_WIDTH, cfg->af);
}

void GPIO_Lock(gpio_port_t port, uint32_t pin_mask)
{
	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);

	gpio->LCKR = (1U << GPIO_LCKK_BIT) | pin_mask;
	gpio->LCKR = pin_mask;
	gpio->LCKR = (1U << GPIO_LCKK_BIT) | pin_mask;

	(void)gpio->LCKR; // read to complete lock sequence, value unused
}

bool GPIO_EnableInterrupt(gpio_port_t port, gpio_pin_t pin, const gpio_interrupt_cfg_t* cfg)
{
	exti_line_t line = get_exti_line(pin);

	if (!SYSCFG_BindEXTILine(line, port))
		return false;

	exti_trigger_cfg_t exti_cfg =
	{
		.line         = line,
		.callback     = cfg->callback,
		.callback_arg = cfg->callback_arg,
		.trigger      = get_exti_trigger_type(cfg->type)
	};

	EXTI_ConfigureInterrupt(&exti_cfg);

	return true;
}

void GPIO_Write(gpio_port_t port, gpio_pin_t pin, gpio_state_t state)
{
	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);

	uint8_t offset = state ? pin : pin + GPIO_BSRR_BR0_OFFSET;
	MMIO_WriteBitsDirect(&gpio->BSRR, offset, 1, 1);
}

void GPIO_Toggle(gpio_port_t port, gpio_pin_t pin)
{
	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);

	uint32_t odr_val = MMIO_ReadBits(&gpio->ODR, pin, 1);
	gpio_state_t state = (gpio_state_t)!!(odr_val);

	GPIO_Write(port, pin, !state);
}

gpio_state_t GPIO_Read(gpio_port_t port, gpio_pin_t pin)
{
	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);

	uint32_t idr_val = MMIO_ReadBits(&gpio->IDR, pin, 1);
	gpio_state_t state = (gpio_state_t)!!(idr_val);

	return state;
}

static exti_line_t get_exti_line(gpio_pin_t pin)
{
	switch (pin)
	{
		case GPIO_PIN_0:
			return EXTI_LINE_0;
		case GPIO_PIN_1:
			return EXTI_LINE_1;
		case GPIO_PIN_2:
			return EXTI_LINE_2;
		case GPIO_PIN_3:
			return EXTI_LINE_3;
		case GPIO_PIN_4:
			return EXTI_LINE_4;
		case GPIO_PIN_5:
			return EXTI_LINE_5;
		case GPIO_PIN_6:
			return EXTI_LINE_6;
		case GPIO_PIN_7:
			return EXTI_LINE_7;
		case GPIO_PIN_8:
			return EXTI_LINE_8;
		case GPIO_PIN_9:
			return EXTI_LINE_9;
		case GPIO_PIN_10:
			return EXTI_LINE_10;
		case GPIO_PIN_11:
			return EXTI_LINE_11;
		case GPIO_PIN_12:
			return EXTI_LINE_12;
		case GPIO_PIN_13:
			return EXTI_LINE_13;
		case GPIO_PIN_14:
			return EXTI_LINE_14;
		case GPIO_PIN_15:
			return EXTI_LINE_15;
		default:
			__builtin_unreachable();
	}
}

static rcc_en_gpio_port_t get_rcc_port(gpio_port_t port)
{
	switch (port)
	{
		case GPIO_PORT_A:
			return RCC_EN_GPIO_PORT_A;
		case GPIO_PORT_B:
			return RCC_EN_GPIO_PORT_B;
		case GPIO_PORT_C:
			return RCC_EN_GPIO_PORT_C;
		case GPIO_PORT_D:
			return RCC_EN_GPIO_PORT_D;
		case GPIO_PORT_H:
			return RCC_EN_GPIO_PORT_H;
		default:
			__builtin_unreachable();
	}
}

static exti_trigger_type_t get_exti_trigger_type(gpio_interrupt_type_t type)
{
	switch (type)
	{
		case GPIO_INTERRUPT_RISING_EDGE:
			return EXTI_TRIGGER_RISING_EDGE;
		case GPIO_INTERRUPT_FALLING_EDGE:
			return EXTI_TRIGGER_FALLING_EDGE;
		case GPIO_INTERRUPT_ANY_EDGE:
			return EXTI_TRIGGER_ANY_EDGE;
		default:
			__builtin_unreachable();
	}
}
