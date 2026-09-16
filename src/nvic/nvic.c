//
// Created by tomly on 16/09/2026.
//

#include <nvic.h>
#include <nvic_internal.h>

#define NVIC_REG(irq_n) ((irq_n) / (NUM_INTERRUPTS_PER_REG))
#define NVIC_FIELD(irq_n) ((irq_n) % (NUM_INTERRUPTS_PER_REG))

void NVIC_EnableInterrupt(irq_t irq_n)
{
    volatile nvic_regs_t* iser = NVIC_ISER_REGS;
    
    uint8_t reg = NVIC_REG(irq_n);
    uint8_t field = NVIC_FIELD(irq_n);
    
    iser->REGS[reg] = 1U << field;
}
