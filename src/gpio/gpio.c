//
// Created by tomly on 02/09/2026.
//

#include <gpio.h>
#include <gpio_internal.h>
#include <mmio.h>
#include <rcc.h>

#define GPIO_AFR_REG(pin) ((pin) / (GPIO_AFR_FIELDS_PER_REG))
#define GPIO_AFR_FIELD(pin) ((pin) % (GPIO_AFR_FIELDS_PER_REG))

static rcc_en_gpio_port_t GPIO_GetRCCPort(gpio_port_t port);

void GPIO_Init(gpio_port_t port, gpio_pin_t pin, const gpio_config_t* cfg) 
{
	rcc_en_gpio_port_t rcc_port = GPIO_GetRCCPort(port);
	RCC_EnableGPIO(rcc_port);
	
	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);
	
	MMIO_WriteField(&gpio->MODER, pin, GPIO_MODE_WIDTH, cfg->mode);
	MMIO_WriteField(&gpio->TYPER, pin, GPIO_TYPE_WIDTH, cfg->type);
	MMIO_WriteField(&gpio->OSPEEDR, pin, GPIO_OSPEED_WIDTH, cfg->ospeed);
	MMIO_WriteField(&gpio->PUPDR, pin, GPIO_PUPD_WIDTH, cfg->pupd);
	MMIO_WriteField(&gpio->AFR[GPIO_AFR_REG(pin)], GPIO_AFR_FIELD(pin), GPIO_AFR_WIDTH, cfg->af);
}

void GPIO_Lock(gpio_port_t port, uint32_t pin_mask) 
{
	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);
	
	gpio->LCKR = (1U << GPIO_LCKK_BIT) | pin_mask;
	gpio->LCKR = pin_mask;
	gpio->LCKR = (1U << GPIO_LCKK_BIT) | pin_mask;
	
	(void)gpio->LCKR; // read to complete lock sequence, value unused
}

void GPIO_Write(gpio_port_t port, gpio_pin_t pin, gpio_state_t state) 
{
	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);
	
	gpio->BSRR = (1U << pin) << (!state * GPIO_BSRR_BR0_OFFSET);
}

void GPIO_Toggle(gpio_port_t port, gpio_pin_t pin) 
{
	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);
	
	gpio_state_t state = (gpio_state_t) !!(gpio->ODR & (1U << pin));
	
	GPIO_Write(port, pin, !state);
}

gpio_state_t GPIO_Read(gpio_port_t port, gpio_pin_t pin) 
{
	volatile gpio_t* gpio = GPIO_PORT_ADDR(port);
	
	gpio_state_t state = (gpio_state_t) !!(gpio->IDR & (1U << pin));
	
	return state;
}

static rcc_en_gpio_port_t GPIO_GetRCCPort(gpio_port_t port)
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
