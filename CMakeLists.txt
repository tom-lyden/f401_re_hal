cmake_minimum_required(VERSION 4.3)
project(f401_re_hal C)

set(CMAKE_C_STANDARD 23)

add_library(f401_re_hal STATIC f401-re-hal.c)
