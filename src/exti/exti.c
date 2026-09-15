//
// Created by tomly on 14/09/2026.
//

#include <exti.h>
#include <exti_internal.h>
#include <mmio.h>
#include <stddef.h>

typedef struct
{
    exti_trigger_callback_t* callback;
    void* callback_arg;
} exti_callback_t;

static exti_callback_t callbacks[EXTI_LINE_MAX];

static void handle_irq_num(exti_line_t exti_line);

void EXTI_ConfigureInterrupt(const exti_trigger_cfg_t* cfg)
{
    volatile exti_t* exti = EXTI_BASE;

    MMIO_WriteField(&exti->IMR, cfg->line, 1, 0);
    
    callbacks[cfg->line].callback_arg = cfg->callback_arg;
    callbacks[cfg->line].callback = cfg->callback;
    
    // Reset to allow reconfiguration in runtime
    MMIO_WriteField(&exti->RTSR, cfg->line, 1, 0);
    MMIO_WriteField(&exti->FTSR, cfg->line, 1, 0);

    if (cfg->trigger & EXTI_TRIGGER_RISING_EDGE)
        MMIO_WriteField(&exti->RTSR, cfg->line, 1, 1);

    if (cfg->trigger & EXTI_TRIGGER_FALLING_EDGE)
        MMIO_WriteField(&exti->FTSR, cfg->line, 1, 1);
    
    exti->PR = 1U << cfg->line;

    MMIO_WriteField(&exti->IMR, cfg->line, 1, 1);
}

// NVIC Implementations
void EXTI0_IRQHandler(void)
{
    handle_irq_num(EXTI_LINE_0);
}

void EXTI1_IRQHandler(void)
{
    handle_irq_num(EXTI_LINE_1);
}

void EXTI2_IRQHandler(void)
{
    handle_irq_num(EXTI_LINE_2);
}

void EXTI3_IRQHandler(void)
{
    handle_irq_num(EXTI_LINE_3);
}

void EXTI4_IRQHandler(void)
{
    handle_irq_num(EXTI_LINE_4);
}

void EXTI9_5_IRQHandler(void)
{
    for (exti_line_t line = EXTI_LINE_5; line <= EXTI_LINE_9; line++)
        handle_irq_num(line);
}

void EXTI15_10_IRQHandler(void)
{
    for (exti_line_t line = EXTI_LINE_10; line <= EXTI_LINE_15; line++)
        handle_irq_num(line);
}

static void handle_irq_num(exti_line_t exti_line)
{
    volatile exti_t* exti = EXTI_BASE;
    
    if (!MMIO_ReadField(&exti->PR, exti_line, 1))
        return;
    
    exti->PR = 1U << exti_line;
    
    if (callbacks[exti_line].callback != NULL)
        callbacks[exti_line].callback(callbacks[exti_line].callback_arg);
}
