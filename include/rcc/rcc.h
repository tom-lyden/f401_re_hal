//
// Created by tomly on 02/09/2026.
//

#ifndef F401_RE_HAL_RCC_H
#define F401_RE_HAL_RCC_H

#include <stdint.h>

typedef enum
{
	RCC_EN_GPIO_PORT_A = 0,
	RCC_EN_GPIO_PORT_B = 1,
	RCC_EN_GPIO_PORT_C = 2,
	RCC_EN_GPIO_PORT_D = 3,

	// Reserved

	RCC_EN_GPIO_PORT_H = 7,
} rcc_en_gpio_port_t;

void RCC_EnableGPIO(rcc_en_gpio_port_t port);
void RCC_EnableSYSCFG(void);

uint32_t RCC_GetHCLK(void);

#endif // F401_RE_HAL_RCC_H
