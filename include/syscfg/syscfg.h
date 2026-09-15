//
// Created by tomly on 15/09/2026.
//

#ifndef F401_RE_HAL_SYSCFG_H
#define F401_RE_HAL_SYSCFG_H

#include <exti.h>
#include <gpio.h>

bool_t SYSCFG_BindEXTILine(exti_line_t line, gpio_port_t port);

#endif // F401_RE_HAL_SYSCFG_H
