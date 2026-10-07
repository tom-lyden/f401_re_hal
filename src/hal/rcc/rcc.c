//
// Created by tomly on 02/09/2026.
//

#include <rcc_internal.h>
#include <rcc.h>
#include <mmio.h>

#include <stdint.h>

static uint32_t scale_clk_ahb(uint32_t sysclk, ahb_scaling_t ahb_scaling);
static uint32_t scale_clk_apb(uint32_t sysclk, apb_scaling_t apb_scaling);

void RCC_EnableGPIO(rcc_en_gpio_port_t port)
{
	volatile rcc_t* rcc = RCC_BASE;

	MMIO_WriteBitsRMW(&rcc->AHB1ENR, port, 1, 1);
}

void RCC_EnableI2C(rcc_en_i2c_port_t port)
{
	volatile rcc_t* rcc = RCC_BASE;
	
	MMIO_WriteBitsRMW(&rcc->APB1ENR, port, 1, 1);
}

void RCC_EnableSYSCFG(void)
{
	volatile rcc_t* rcc = RCC_BASE;

	MMIO_WriteBitsRMW(&rcc->APB1ENR, RCC_APB2EN_SYSCFG_OFFSET, 1, 1);
}

uint32_t RCC_GetHCLK(void)
{
	volatile rcc_t* rcc = RCC_BASE;

	uint32_t cfgr = rcc->CFGR;

	sysclk_t sysclk = MMIO_ReadBits(&cfgr, RCC_CFG_SWS_OFFSET, RCC_CFG_SWS_WIDTH);
	ahb_scaling_t ahb_scaling = MMIO_ReadBits(&cfgr, RCC_CFG_HPRE_OFFSET, RCC_CFG_HPRE_WIDTH);

	switch (sysclk)
	{
		case SYSCLK_HSI:
			return scale_clk_ahb(HSI_FREQ_HZ, ahb_scaling);
		case SYSCLK_HSE:
		case SYSCLK_PLL:
		default:
			while (1) { } // Not implemented yet; spin forever
	}
}

uint32_t RCC_GetPCLK1(void)
{
	uint32_t sysclk = RCC_GetHCLK();

	volatile rcc_t* rcc = RCC_BASE;
	uint32_t cfgr = rcc->CFGR;
	
	apb_scaling_t apb_scaling = MMIO_ReadBits(&cfgr, RCC_CFG_PPRE1_OFFSET, RCC_CFG_PPRE1_WIDTH);
	
	uint32_t pclk1 = scale_clk_apb(sysclk, apb_scaling);
	
	return pclk1;
}

static uint32_t scale_clk_ahb(uint32_t sysclk, ahb_scaling_t ahb_scaling)
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

static uint32_t scale_clk_apb(uint32_t sysclk, apb_scaling_t apb_scaling)
{
	switch (apb_scaling)
	{
		case APB_DIV_2:
			return sysclk >> 1;
		case APB_DIV_4:
			return sysclk >> 2;
		case APB_DIV_8:
			return sysclk >> 3;
		case APB_DIV_16:
			return sysclk >> 4;
		default:
			return sysclk;
	}
}
