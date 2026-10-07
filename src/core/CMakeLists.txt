target_sources(f401_re_hal 
        PRIVATE
        cortex_m4/nvic/nvic.c
        cortex_m4/systick/systick.c
)

target_include_directories(f401_re_hal 
        PRIVATE
        cortex_m4/nvic
        cortex_m4/systick
)
