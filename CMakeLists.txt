cmake_minimum_required(VERSION 4.3)
project(f401_re_hal C)

set(CMAKE_C_STANDARD 23)

add_library(f401_re_hal STATIC
        src/gpio/gpio.c
        src/rcc/rcc.c
)

target_include_directories(f401_re_hal 
        PUBLIC
        include
        
        PRIVATE
        include/gpio
        include/mmio
        include/rcc
)
