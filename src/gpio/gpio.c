//
// Created by tomly on 02/09/2026.
//

#include <gpio.h>
#include <gpio_internal.h>
#include <rcc.h>

static rcc_en_gpio_port_t GPIO_GetRCCPort(gpio_port_t port);

void GPIO_Init(gpio_port_t port, gpio_pin_t pin, const gpio_config_t* cfg) 
{
	rcc_en_gpio_port_t rcc_port = GPIO_GetRCCPort(port);
	RCC_EnableGPIO(rcc_port);
}

void GPIO_Lock(gpio_port_t port, gpio_pin_t pin) 
{
	
}

void GPIO_Write(gpio_port_t port, gpio_pin_t pin, gpio_state_t state) 
{
	
}

void GPIO_Toggle(gpio_port_t port, gpio_pin_t pin) 
{
	
}

gpio_state_t GPIO_Read(gpio_port_t port, gpio_pin_t pin) 
{
	
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
