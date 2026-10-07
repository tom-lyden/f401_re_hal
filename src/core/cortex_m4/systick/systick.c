//
// Created by tomly on 06/09/2026.
//

#include <systick_internal.h>
#include <systick.h>
#include <mmio.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#define SYSTICK_AHB_DIV(val) ((val) / 8)

static systick_handler_t handler = NULL;

bool SysTick_Init(const systick_cfg_t* cfg)
{
	assert(cfg != NULL);

	uint32_t period_cycles = cfg->period_cycles;
	systick_clk_src_t clk_src = period_cycles > SYSTICK_PERIOD_MAX_CYCLES
		? SYSTICK_CLK_SRC_AHB_DIV_8
		: SYSTICK_CLK_SRC_AHB;

	switch (clk_src)
	{
		case SYSTICK_CLK_SRC_AHB_DIV_8:
			period_cycles = SYSTICK_AHB_DIV(period_cycles);
			break;
		case SYSTICK_CLK_SRC_AHB:
			break;
		default:
			__builtin_unreachable();
	}

	if (period_cycles < 2 || period_cycles > SYSTICK_PERIOD_MAX_CYCLES)
		return false;

	handler = cfg->enable_irq ? cfg->irq_handler : NULL;

	volatile systick_t* systick = SYSTICK_BASE;

	uint32_t reload = period_cycles - 1;
	MMIO_WriteBitsRMW(&systick->LOAD, 0, SYSTICK_COUNTER_WIDTH, reload);

	systick->VAL = 0U;

	MMIO_WriteBitsRMW(&systick->CTRL, SYSTICK_CTRL_TICKINT_OFFSET, 1, cfg->enable_irq);
	MMIO_WriteBitsRMW(&systick->CTRL, SYSTICK_CTRL_CLKSOURCE_OFFSET, 1, clk_src);
	MMIO_WriteBitsRMW(&systick->CTRL, SYSTICK_CTRL_ENABLE_OFFSET, 1, 1);

	return true;
}

void SysTick_Handler(void)
{
	if (handler != NULL)
		handler();
}
