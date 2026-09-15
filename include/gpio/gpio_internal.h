//
// Created by tomly on 02/09/2026.
//

#ifndef F401_RE_HAL_GPIO_INTERNAL_H
#define F401_RE_HAL_GPIO_INTERNAL_H

#include <stdint.h>

#define GPIO_BASE_ADDR (0x40020000U)
#define GPIO_PORT_WIDTH (0x0400U)

#define GPIO_PORT_ADDR(port) ((volatile gpio_t *)((GPIO_BASE_ADDR) + ((port) * (GPIO_PORT_WIDTH))))

#define GPIO_LCKK_BIT (16)

#define GPIO_MODE_WIDTH (2)
#define GPIO_TYPE_WIDTH (1)
#define GPIO_OSPEED_WIDTH (2)
#define GPIO_PUPD_WIDTH (2)
#define GPIO_ODR_WIDTH (1)
#define GPIO_AFR_WIDTH (4)

#define GPIO_AFR_FIELDS_PER_REG (8)

#define GPIO_BSRR_BR0_OFFSET (16)

typedef struct
{
	uint32_t MODER;
	uint32_t TYPER;
	uint32_t OSPEEDR;
	uint32_t PUPDR;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t LCKR;
	uint32_t AFR[2];
} gpio_t;


#endif // F401_RE_HAL_GPIO_INTERNAL_H
