//
// Created by tomly on 06/09/2026.
//

#ifndef F401_RE_HAL_SYSTICK_H
#define F401_RE_HAL_SYSTICK_H

#include <utils.h>
#include <stdint.h>

typedef enum
{
	SYSTICK_CLK_SRC_AHB_DIV_8 = 0,
	SYSTICK_CLK_SRC_AHB = 1,
} systick_clk_src_t;

typedef struct
{
	uint32_t tick_freq;
	systick_clk_src_t clk_src;
	bool_t enable_irq;
	bool_t enable_counter;
} systick_cfg_t;

bool_t SysTick_Init(const systick_cfg_t* cfg);
uint32_t SysTick_GetTick(void);

#endif // F401_RE_HAL_SYSTICK_H
