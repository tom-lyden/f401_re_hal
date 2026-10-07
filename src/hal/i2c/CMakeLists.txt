target_sources(f401_re_hal 
        PRIVATE
        i2c_fsm/i2c_fsm_ctrl_write.c
        i2c_fsm.c
        i2c_internal.c
        i2c_queue.c
        i2c.c
)

target_include_directories(f401_re_hal
        PRIVATE
        .
        i2c_fsm
)
