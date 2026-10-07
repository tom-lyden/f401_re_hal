//
// Created by tomly on 04/10/2026.
//

#ifndef F401_RE_HAL_I2C_FSM_H
#define F401_RE_HAL_I2C_FSM_H

#include <i2c.h>

typedef enum
{
	I2C_FSM_STATE_IDLE = 0,
	I2C_FSM_STATE_START,
	I2C_FSM_STATE_ADDR,
	I2C_FSM_STATE_DATA,
	I2C_FSM_STATE_STOP,
	I2C_FSM_STATE_DONE,
	I2C_FSM_STATE_ERROR,
	I2C_FSM_NUM_STATES
} i2c_fsm_state_t;

typedef struct i2c_fsm_s i2c_fsm_t;

typedef struct
{
	i2c_fsm_t* fsm;
	i2c_bus_t bus;
	i2c_req_t* req;
	uint32_t byte_counter;
} i2c_callback_ctx_t;

typedef void (*i2c_fsm_callback_t)(i2c_callback_ctx_t *);

typedef struct 
{
	i2c_fsm_callback_t enter;
	i2c_fsm_callback_t handle_evt;
	i2c_fsm_callback_t update;
} i2c_fsm_state_callbacks_t;

typedef struct i2c_fsm_s
{
	volatile i2c_fsm_state_t state;
	const i2c_fsm_state_callbacks_t* callbacks;
	i2c_callback_ctx_t ctx;
} i2c_fsm_t;

void I2C_FSM_Init(i2c_fsm_t* fsm, const i2c_fsm_state_callbacks_t* callbacks, i2c_callback_ctx_t ctx);
void I2C_FSM_SetState(i2c_fsm_t* fsm, i2c_fsm_state_t state);
void I2C_FSM_Update(i2c_fsm_t* fsm);
void I2C_FSM_Handle_Event(i2c_fsm_t* fsm);
void I2C_FSM_Handle_Error(i2c_fsm_t* fsm);

#endif // F401_RE_HAL_I2C_FSM_H
