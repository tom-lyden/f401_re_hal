//
// Created by tomly on 16/09/2026.
//

#ifndef F401_RE_HAL_NVIC_INTERNAL_H
#define F401_RE_HAL_NVIC_INTERNAL_H

#include <limits.h>
#include <stdint.h>

#define NVIC_BASE_ADDR (0xE000E100)

#define NVIC_REGS_ADDR(regs_offset) ((volatile nvic_regs_t *)((NVIC_BASE_ADDR) + (regs_offset)))

#define NVIC_ISER_OFFSET (0x000)

#define NVIC_ISER_REGS (NVIC_REGS_ADDR(NVIC_ISER_OFFSET))

#define NUM_INTERRUPTS (240)
#define NUM_INTERRUPTS_PER_REG (sizeof (uint32_t) * CHAR_BIT)
#define NUM_REGS (((NUM_INTERRUPTS) / (NUM_INTERRUPTS_PER_REG)) + (((NUM_INTERRUPTS) % (NUM_INTERRUPTS_PER_REG)) ? 1 : 0))

typedef struct
{
	uint32_t REGS[NUM_REGS];
} nvic_regs_t;

#endif //F401_RE_HAL_NVIC_INTERNAL_H
