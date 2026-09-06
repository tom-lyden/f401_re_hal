//
// Created by tomly on 02/09/2026.
//

#include <rcc-internal.h>
#include <rcc.h>

#include "mmio.h"

static uint32_t RCC_ScaleSysClk(uint32_t sysclk, ahb_scaling_t ahb_scaling);

void RCC_EnableGPIO(rcc_en_gpio_port_t port)
{
	volatile rcc_t* rcc = RCC_BASE;
	
	rcc->AHB1ENR |= 1U << port;
}

uint32_t RCC_GetHCLK(void)
{
	volatile rcc_t* rcc = RCC_BASE;
	
	uint32_t cfgr = rcc->CFGR;
	
	sysclk_t sysclk = MMIO_ReadField(&cfgr, RCC_CFG_SWS_OFFSET, RCC_CFG_SWS_WIDTH);
	ahb_scaling_t ahb_scaling = MMIO_ReadField(&cfgr, RCC_CFG_HPRE_OFFSET, RCC_CFG_HPRE_WIDTH);
	
	switch (sysclk)
	{
		case SYSCLK_HSI:
			return RCC_ScaleSysClk(HSI_FREQ_HZ, ahb_scaling);
		case SYSCLK_HSE:
		case SYSCLK_PLL:
		default:
			while (1) {} // Not implemented yet; spin forever
	}
}

static uint32_t RCC_ScaleSysClk(uint32_t sysclk, ahb_scaling_t ahb_scaling)
{
	switch (ahb_scaling)
	{
		case AHB_DIV_2:
			return sysclk >> 1;
		case AHB_DIV_4:
			return sysclk >> 2;
		case AHB_DIV_8:
			return sysclk >> 3;
		case AHB_DIV_16:
			return sysclk >> 4;
		case AHB_DIV_64:
			return sysclk >> 6;
		case AHB_DIV_128:
			return sysclk >> 7;
		case AHB_DIV_256:
			return sysclk >> 8;
		case AHB_DIV_512:
			return sysclk >> 9;
		default:
			return sysclk;
	}
}
