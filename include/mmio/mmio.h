//
// Created by tomly on 03/09/2026.
//

#ifndef F401_RE_HAL_MMIO_H
#define F401_RE_HAL_MMIO_H

#include <assert.h>
#include <limits.h>
#include <stdint.h>

#define REG_BITS (sizeof(uint32_t) * CHAR_BIT)

static inline void MMIO_WriteBitsRMW(volatile uint32_t* reg, uint8_t offset, uint8_t width, uint32_t value)
{
	assert(width > 0);
	assert(width <= REG_BITS);
	assert(offset <= REG_BITS - width);
	assert(value <= (1ULL << width) - 1);
	
	uint32_t mask = width == REG_BITS ? (uint32_t)(-1) : (1U << width) - 1;
	uint32_t shift = width == REG_BITS ? 0 : offset;

	uint32_t temp = *reg;
	temp &= ~(mask << shift);
	temp |= (value & mask) << shift;
	*reg = temp;
}

static inline void MMIO_WriteBitsDirect(volatile uint32_t* reg, uint8_t offset, uint8_t width, uint32_t value)
{
	assert(width > 0);
	assert(width <= REG_BITS);
	assert(offset <= REG_BITS - width);
	assert(value <= (1ULL << width) - 1);
	
	uint32_t mask = width == REG_BITS ? (uint32_t)(-1) : (1U << width) - 1;
	uint32_t shift = width == REG_BITS ? 0 : offset;
	
	*reg =  (value & mask) << shift;
}

static inline uint32_t MMIO_ReadBits(volatile uint32_t* reg, uint8_t offset, uint8_t width)
{
	assert(width > 0);
	assert(width <= REG_BITS);
	assert(offset <= REG_BITS - width);
	
	uint32_t mask = width == REG_BITS ? (uint32_t)(-1) : (1U << width) - 1;
	uint32_t shift = width == REG_BITS ? 0 : offset;

	uint32_t temp = *reg;
	temp >>= shift;
	temp &= mask;
	return temp;
}

#endif // F401_RE_HAL_MMIO_H
