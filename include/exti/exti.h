//
// Created by tomly on 14/09/2026.
//

#ifndef F401_RE_HAL_EXTI_H
#define F401_RE_HAL_EXTI_H

typedef enum
{
    EXTI_LINE_0 = 0,
    EXTI_LINE_1,
    EXTI_LINE_2,
    EXTI_LINE_3,
    EXTI_LINE_4,
    EXTI_LINE_5,
    EXTI_LINE_6,
    EXTI_LINE_7,
    EXTI_LINE_8,
    EXTI_LINE_9,
    EXTI_LINE_10,
    EXTI_LINE_11,
    EXTI_LINE_12,
    EXTI_LINE_13,
    EXTI_LINE_14,
    EXTI_LINE_15,
    EXTI_LINE_MAX
} exti_line_t;

typedef enum
{
    EXTI_TRIGGER_RISING_EDGE = 1,
    EXTI_TRIGGER_FALLING_EDGE = 2,
    EXTI_TRIGGER_ANY_EDGE = 3,
} exti_trigger_type_t;

typedef void exti_trigger_callback_t(void *);

typedef struct
{
    exti_line_t line;
    exti_trigger_callback_t* callback;
    void* callback_arg;
    exti_trigger_type_t trigger;
} exti_trigger_cfg_t;

void EXTI_ConfigureInterrupt(const exti_trigger_cfg_t* cfg);

#endif //F401_RE_HAL_EXTI_H
