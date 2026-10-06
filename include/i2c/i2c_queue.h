//
// Created by tomly on 04/10/2026.
//

#ifndef F401_RE_HAL_I2C_QUEUE_H
#define F401_RE_HAL_I2C_QUEUE_H

#include <i2c.h>

#define MAX_I2C_REQS (32)

typedef struct
{
	i2c_req_t* reqs[MAX_I2C_REQS];
	uint32_t num_reqs;
	uint32_t head;
	uint32_t tail;
} i2c_queue_t;

void I2C_Queue_Init(i2c_queue_t* queue);
bool I2C_Queue_Enqueue(i2c_queue_t* queue, i2c_req_t* req);
bool I2C_Queue_Dequeue(i2c_queue_t* queue, i2c_req_t** req);
bool I2C_Queue_Peek(const i2c_queue_t* queue, i2c_req_t** req);
bool I2C_Queue_IsEmpty(const i2c_queue_t* queue);
bool I2C_Queue_IsFull(const i2c_queue_t* queue);

#endif // F401_RE_HAL_I2C_QUEUE_H
