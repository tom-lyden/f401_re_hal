target_sources(f401_re_hal 
        PRIVATE
        stm32f401re_startup.c
)

target_include_directories(f401_re_hal
        PRIVATE
        .
)
