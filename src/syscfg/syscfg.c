//
// Created by tomly on 15/09/2026.
//

#include <stdbool.h>
#include <stdint.h>
#include <mmio.h>
#include <rcc.h>
#include <syscfg.h>
#include <syscfg_internal.h>

#define NUM_EXTI_GPIO_LINES (EXTI_LINE_15 + 1)
#define NUM_EXTI_LINES_PER_REGISTER ((NUM_EXTI_GPIO_LINES) / (NUM_EXTICR))

#define LINE_REGISTER(line) ((line) / (NUM_EXTI_LINES_PER_REGISTER))
#define LINE_INDEX(line) ((line) % (NUM_EXTI_LINES_PER_REGISTER))

static bool is_exti_line_occupied[NUM_EXTI_GPIO_LINES] = { false };

bool SYSCFG_BindEXTILine(exti_line_t line, gpio_port_t port)
{
	if (is_exti_line_occupied[line])
		return false;

	RCC_EnableSYSCFG();

	volatile syscfg_t* syscfg = SYSCFG_BASE;

	uint32_t line_reg = LINE_REGISTER(line);
	uint32_t line_offset = LINE_INDEX(line);

	MMIO_WriteField(&syscfg->EXTICR[line_reg], line_offset, EXTI_LINE_FIELD_WIDTH, port);

	is_exti_line_occupied[line] = true;

	return true;
}
