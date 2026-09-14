//
// Created by tomly on 14/09/2026.
//

#ifndef F401_RE_HAL_EXTI_INTERNAL_H
#define F401_RE_HAL_EXTI_INTERNAL_H

#include <stdint.h>

#define EXTI_BASE_ADDR (0x40013C00U)
#define EXTI_BASE ((volatile exti_t *)EXTI_BASE_ADDR)

typedef struct
{
    uint32_t IMR;
    uint32_t EMR;
    uint32_t RTSR;
    uint32_t FTSR;
    uint32_t SWIER;
    uint32_t PR;
} exti_t;

#endif //F401_RE_HAL_EXTI_INTERNAL_H
