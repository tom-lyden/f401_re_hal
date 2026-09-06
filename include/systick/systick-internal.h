//
// Created by tomly on 06/09/2026.
//

#ifndef F401_RE_HAL_SYSTICK_INTERNAL_H
#define F401_RE_HAL_SYSTICK_INTERNAL_H

#include <stdint.h>

#define SYSTICK_BASE_ADDR (0xE000E010U)

#define SYSTICK_BASE ((volatile systick_t *)(SYSTICK_BASE_ADDR))

#define SYSTICK_CTRL_ENABLE_OFFSET (0)
#define SYSTICK_CTRL_TICKINT_OFFSET (1)
#define SYSTICK_CTRL_CLKSOURCE_OFFSET (2)
#define SYSTICK_CTRL_COUNTFLAG_OFFSET (16)

#define SYSTICK_COUNTER_WIDTH (24)
#define SYSTICK_PERIOD_MAX_CYCLES (1U << (SYSTICK_COUNTER_WIDTH))

typedef struct systick
{
	uint32_t CTRL;
	uint32_t LOAD;
	uint32_t VAL;
	uint32_t CALIB;
} systick_t;

#endif // F401_RE_HAL_SYSTICK_INTERNAL_H
