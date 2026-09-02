cmake_minimum_required(VERSION 4.3)
project(f401_re_hal C)

set(CMAKE_C_STANDARD 23)

add_library(f401_re_hal STATIC src/f401-re-hal.c
        src/gpio
        src/rcc
)

target_include_directories(f401_re_hal PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        
        include/gpio
        include/rcc
)
