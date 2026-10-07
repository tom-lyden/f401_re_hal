//
// Created by tomly on 04/10/2026.
//

#include <i2c_fsm.h>

void I2C_FSM_Init(i2c_fsm_t* fsm, const i2c_fsm_state_callbacks_t* callbacks, i2c_callback_ctx_t ctx)
{
	fsm->callbacks = callbacks;
	fsm->state = I2C_FSM_STATE_IDLE;
	fsm->ctx = ctx;
}

void I2C_FSM_SetState(i2c_fsm_t* fsm, i2c_fsm_state_t state)
{
	fsm->state = state;
	fsm->callbacks[fsm->state].enter(&fsm->ctx);
}

void I2C_FSM_Update(i2c_fsm_t* fsm)
{
	fsm->callbacks[fsm->state].update(&fsm->ctx);
}

void I2C_FSM_Handle_Event(i2c_fsm_t* fsm)
{
	fsm->callbacks[fsm->state].handle_evt(&fsm->ctx);
}

void I2C_FSM_Handle_Error(i2c_fsm_t* fsm)
{
	I2C_FSM_SetState(fsm, I2C_FSM_STATE_ERROR);
}
