cmake_minimum_required(VERSION 4.3)
project(f401_re_hal C)

set(CMAKE_C_STANDARD 11)

add_library(f401_re_hal)

target_include_directories(f401_re_hal
        PUBLIC
        include
)

add_subdirectory(src/common)
add_subdirectory(src/core)
add_subdirectory(src/device)
add_subdirectory(src/hal)

target_include_directories(f401_re_hal
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/device
)

target_sources(f401_re_hal
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/device/stm32f401re_startup.c
)

set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/src/device/stm32f401re_startup.c
        PROPERTIES COMPILE_OPTIONS "-fno-tree-loop-distribute-patterns;"
)

target_link_options(f401_re_hal
        INTERFACE
        "LINKER:-T,${CMAKE_CURRENT_LIST_DIR}/src/device/stm32f401re_flash.ld"
        "-nostartfiles"
)
