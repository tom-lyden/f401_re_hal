//
// Created by tomly on 16/09/2026.
//

#include <nvic_internal.h>
#include <nvic.h>
#include <mmio.h>

#include <stdint.h>
#include <assert.h>

#define NVIC_REG(irq_n) ((irq_n) / (NUM_INTERRUPTS_PER_REG))
#define NVIC_FIELD(irq_n) ((irq_n) % (NUM_INTERRUPTS_PER_REG))

void NVIC_EnableInterrupt(uint8_t irq_n)
{
	assert(irq_n < NUM_INTERRUPTS);

	volatile nvic_regs_t* iser = NVIC_ISER_REGS;

	uint8_t reg = NVIC_REG(irq_n);
	uint8_t field = NVIC_FIELD(irq_n);

	MMIO_WriteBitsDirect(&iser->REGS[reg], field, 1, 1);
}
