//
// Created by tomly on 02/09/2026.
//

#include <rcc-internal.h>
#include <rcc.h>

void RCC_EnableGPIO(rcc_en_gpio_port_t port)
{
	volatile rcc_t* rcc = RCC_BASE;
	
	rcc->AHB1ENR |= 1U << port;
}
