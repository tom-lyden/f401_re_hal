//
// Created by tomly on 15/09/2026.
//

#include <syscfg_internal.h>
#include <syscfg.h>
#include <rcc.h>
#include <mmio.h>

#include <stdbool.h>
#include <stdint.h>

#define NUM_EXTI_GPIO_LINES (EXTI_LINE_15 + 1)
#define NUM_EXTI_LINES_PER_REGISTER ((NUM_EXTI_GPIO_LINES) / (NUM_EXTICR))

#define LINE_REGISTER(line) ((line) / (NUM_EXTI_LINES_PER_REGISTER))
#define LINE_INDEX(line) ((line) % (NUM_EXTI_LINES_PER_REGISTER))

typedef struct
{
	bool is_claimed;
	gpio_port_t owner;
} exti_line_owner_t;

// Tracked in software: the EXTICR reset value (0) is also port A, so it can't tell "unclaimed" from "PA"
static exti_line_owner_t line_owners[NUM_EXTI_GPIO_LINES];

bool SYSCFG_BindEXTILine(exti_line_t line, gpio_port_t port)
{
	if (line_owners[line].is_claimed)
		return line_owners[line].owner == port; // Owner keeps its routing; any other port is rejected

	RCC_EnableSYSCFG();

	volatile syscfg_t* syscfg = SYSCFG_BASE;

	uint32_t line_reg = LINE_REGISTER(line);
	uint32_t line_offset = LINE_INDEX(line) * EXTI_LINE_FIELD_WIDTH;

	MMIO_WriteBitsRMW(&syscfg->EXTICR[line_reg], line_offset, EXTI_LINE_FIELD_WIDTH, port);

	line_owners[line].is_claimed = true;
	line_owners[line].owner = port;

	return true;
}

bool SYSCFG_UnbindEXTILine(exti_line_t line, gpio_port_t port)
{
	if (!line_owners[line].is_claimed || line_owners[line].owner != port)
		return false;

	line_owners[line].is_claimed = false; // EXTICR routing is left as is; the next bind overwrites it

	return true;
}
