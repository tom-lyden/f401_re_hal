target_sources(f401_re_hal 
        PRIVATE
        exti/exti.c
        gpio/gpio.c
        rcc/rcc.c
        syscfg/syscfg.c
        time/ticks.c
)

target_include_directories(f401_re_hal
        PUBLIC
        gpio
        time
        
        PRIVATE
        exti
        rcc
        syscfg
)

add_subdirectory(i2c)
