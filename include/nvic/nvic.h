//
// Created by tomly on 16/09/2026.
//

#ifndef F401_RE_HAL_NVIC_H
#define F401_RE_HAL_NVIC_H

typedef enum
{
    IRQ_EXTI_LINE_0 = 6,
    IRQ_EXTI_LINE_1,
    IRQ_EXTI_LINE_2,
    IRQ_EXTI_LINE_3,
    IRQ_EXTI_LINE_4,
    IRQ_EXTI_LINE_9_5 = 23,
    IRQ_EXTI_LINE_15_10 = 40,
} irq_t;

void NVIC_EnableInterrupt(irq_t irq_n);

#endif //F401_RE_HAL_NVIC_H
