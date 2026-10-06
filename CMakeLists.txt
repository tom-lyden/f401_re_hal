cmake_minimum_required(VERSION 4.3)
project(f401_re_hal C)

set(CMAKE_C_STANDARD 11)

add_library(f401_re_hal STATIC
        src/gpio/gpio.c
        src/rcc/rcc.c
        src/systick/systick.c
        src/exti/exti.c
        src/syscfg/syscfg.c
        src/nvic/nvic.c
        src/i2c/i2c.c
        src/i2c/i2c_internal.c
        src/i2c/i2c_fsm.c
        src/i2c/i2c_queue.c
        src/i2c/i2c_fsm/i2c_fsm_ctrl_write.c
)

target_include_directories(f401_re_hal
        PUBLIC
        include
        device

        PRIVATE
        include/gpio
        include/mmio
        include/rcc
        include/systick
        include/exti
        include/syscfg
        include/nvic
        include/i2c
        include/i2c/i2c_fsm
)
