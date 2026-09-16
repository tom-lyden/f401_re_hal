//
// Created by tomly on 14/09/2026.
//

#include <exti.h>
#include <exti_internal.h>
#include <mmio.h>
#include <nvic.h>
#include <stddef.h>

typedef struct
{
	exti_trigger_callback_t* callback;
	void* callback_arg;
} exti_callback_t;

static exti_callback_t callbacks[EXTI_LINE_MAX];

static void handle_irq_on_line(exti_line_t exti_line);
static irq_t get_nvic_irq_number(exti_line_t line);

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

	NVIC_EnableInterrupt(get_nvic_irq_number(cfg->line));

	MMIO_WriteField(&exti->IMR, cfg->line, 1, 1);
}

void EXTI0_Handler(void)
{
	handle_irq_on_line(EXTI_LINE_0);
}

void EXTI1_Handler(void)
{
	handle_irq_on_line(EXTI_LINE_1);
}

void EXTI2_Handler(void)
{
	handle_irq_on_line(EXTI_LINE_2);
}

void EXTI3_Handler(void)
{
	handle_irq_on_line(EXTI_LINE_3);
}

void EXTI4_Handler(void)
{
	handle_irq_on_line(EXTI_LINE_4);
}

void EXTI9_5_Handler(void)
{
	for (exti_line_t line = EXTI_LINE_5; line <= EXTI_LINE_9; line++)
		handle_irq_on_line(line);
}

void EXTI15_10_Handler(void)
{
	for (exti_line_t line = EXTI_LINE_10; line <= EXTI_LINE_15; line++)
		handle_irq_on_line(line);
}

static void handle_irq_on_line(exti_line_t exti_line)
{
	volatile exti_t* exti = EXTI_BASE;

	if (!MMIO_ReadField(&exti->PR, exti_line, 1))
		return;

	exti->PR = 1U << exti_line;

	if (callbacks[exti_line].callback != NULL)
		callbacks[exti_line].callback(callbacks[exti_line].callback_arg);
}

static irq_t get_nvic_irq_number(exti_line_t line)
{
	switch (line)
	{
		case EXTI_LINE_0:
			return IRQ_EXTI_LINE_0;
		case EXTI_LINE_1:
			return IRQ_EXTI_LINE_1;
		case EXTI_LINE_2:
			return IRQ_EXTI_LINE_2;
		case EXTI_LINE_3:
			return IRQ_EXTI_LINE_3;
		case EXTI_LINE_4:
			return IRQ_EXTI_LINE_4;
		case EXTI_LINE_5:
		case EXTI_LINE_6:
		case EXTI_LINE_7:
		case EXTI_LINE_8:
		case EXTI_LINE_9:
			return IRQ_EXTI_LINE_9_5;
		case EXTI_LINE_10:
		case EXTI_LINE_11:
		case EXTI_LINE_12:
		case EXTI_LINE_13:
		case EXTI_LINE_14:
		case EXTI_LINE_15:
			return IRQ_EXTI_LINE_15_10;
		default:
			__builtin_unreachable();
	}
}
