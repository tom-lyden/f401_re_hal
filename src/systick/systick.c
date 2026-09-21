//
// Created by tomly on 06/09/2026.
//

#include <stdint.h>
#include <stdbool.h>
#include <systick.h>
#include <systick_internal.h>
#include <rcc.h>
#include <mmio.h>

#define SYSTICK_AHB_DIV(val) ((val) / 8)

static volatile uint32_t counter = 0U;

bool SysTick_Init(const systick_cfg_t* cfg)
{
	uint32_t hclk = RCC_GetHCLK();

	switch (cfg->clk_src)
	{
		case SYSTICK_CLK_SRC_AHB_DIV_8:
			hclk = SYSTICK_AHB_DIV(hclk);
			break;
		case SYSTICK_CLK_SRC_AHB:
			break;
		default:
			__builtin_unreachable();
	}

	uint32_t period_cycles = hclk / cfg->tick_freq;

	if (period_cycles < 2 || period_cycles > SYSTICK_PERIOD_MAX_CYCLES)
		return false;

	volatile systick_t* systick = SYSTICK_BASE;

	uint32_t reload = period_cycles - 1;
	MMIO_WriteField(&systick->LOAD, 0, SYSTICK_COUNTER_WIDTH, reload);

	systick->VAL = 0U;

	MMIO_WriteField(&systick->CTRL, SYSTICK_CTRL_TICKINT_OFFSET, 1, cfg->enable_irq);
	MMIO_WriteField(&systick->CTRL, SYSTICK_CTRL_CLKSOURCE_OFFSET, 1, cfg->clk_src);
	MMIO_WriteField(&systick->CTRL, SYSTICK_CTRL_ENABLE_OFFSET, 1, cfg->enable_counter);

	return true;
}

uint32_t SysTick_GetTick(void)
{
	return counter;
}

void SysTick_Handler(void)
{
	counter++;
}
