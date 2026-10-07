//
// Created by tomly on 06/09/2026.
//

#ifndef F401_RE_HAL_SYSTICK_H
#define F401_RE_HAL_SYSTICK_H

#include <stdbool.h>
#include <stdint.h>

typedef void (*systick_handler_t)(void);

typedef struct
{
	uint32_t period_cycles;
	systick_handler_t irq_handler;
	bool enable_irq;
} systick_cfg_t;

bool SysTick_Init(const systick_cfg_t* cfg);

#endif // F401_RE_HAL_SYSTICK_H
