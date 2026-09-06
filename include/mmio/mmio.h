//
// Created by tomly on 03/09/2026.
//

#ifndef F401_RE_HAL_MMIO_H
#define F401_RE_HAL_MMIO_H

#include <stdint.h>

static inline void MMIO_WriteField(volatile uint32_t *reg, uint32_t field, uint8_t width, uint32_t value)
{
	uint32_t mask = width == 32 ? (uint32_t)(-1) : (1U << width) - 1;
	uint32_t shift = width == 32 ? 0 : field * width;
	
	uint32_t temp = *reg;
	temp &= ~(mask << shift);
	temp |= (value & mask) << shift;
	*reg = temp;
}

static inline uint32_t MMIO_ReadField(volatile uint32_t *reg, uint32_t field, uint32_t width)
{
	uint32_t mask = width == 32 ? (uint32_t)(-1) : (1U << width) - 1;
	uint32_t shift = width == 32 ? 0 : field * width;
	
	uint32_t temp = *reg;
	temp >>= shift;
	temp &= mask;
	return temp;
}

#endif // F401_RE_HAL_MMIO_H
