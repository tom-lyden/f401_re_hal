//
// Created by tomly on 02/09/2026.
//

#include <rcc-internal.h>
#include <rcc.h>
#include <stdint.h>

void RCC_EnableGPIO(rcc_en_gpio_port_t port)
{
	volatile uint32_t* rcc_enable_reg = RCC_AHB1_ENABLER;
	
	*rcc_enable_reg |= 1U << port;
}
