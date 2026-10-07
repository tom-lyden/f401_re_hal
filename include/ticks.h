//
// Created by tomly on 07/10/2026.
//

#ifndef F401_RE_HAL_TICKS_H
#define F401_RE_HAL_TICKS_H

#include <stdint.h>
#include <stdbool.h>

bool Ticks_Init(uint32_t tick_frequency_hz);
uint32_t Ticks_GetTick(void);

#endif // F401_RE_HAL_TICKS_H
