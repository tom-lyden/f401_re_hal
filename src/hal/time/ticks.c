//
// Created by tomly on 07/10/2026.
//

#include <ticks.h>
#include <rcc.h>

#include <systick.h>

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

static void increment_counter(void);

static volatile uint32_t counter = 0U;

bool Ticks_Init(uint32_t tick_frequency_hz)
{
	assert(tick_frequency_hz > 0U);

	uint32_t hclk = RCC_GetHCLK();

	uint32_t period_cycles = hclk / tick_frequency_hz;

	systick_cfg_t systick_cfg =
	{
		.period_cycles = period_cycles,
		.enable_irq    = true,
		.irq_handler   = increment_counter,
	};

	return SysTick_Init(&systick_cfg);
}

uint32_t Ticks_GetTick(void)
{
	return counter;
}

static void increment_counter(void)
{
	counter++;
}
