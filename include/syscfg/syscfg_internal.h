//
// Created by tomly on 15/09/2026.
//

#ifndef F401_RE_HAL_SYSCFG_INTERNAL_H
#define F401_RE_HAL_SYSCFG_INTERNAL_H

#include <stdint.h>

#define SYSCFG_BASE_ADDR (0x40013800U)
#define SYSCFG_BASE ((volatile syscfg_t *)SYSCFG_BASE_ADDR)

#define NUM_EXTICR (4)
#define EXTI_LINE_FIELD_WIDTH (4)

typedef struct
{
	uint32_t MEMRMP;
	uint32_t PMC;
	uint32_t EXTICR[NUM_EXTICR];
	uint32_t CMPCR;
} syscfg_t;

#endif // F401_RE_HAL_SYSCFG_INTERNAL_H
