//
// Created by tomly on 04/10/2026.
//

#include <i2c_queue.h>

void I2C_Queue_Init(i2c_queue_t* queue)
{
	queue->num_reqs = 0;
	queue->head = 0;
	queue->tail = 0;
}

bool I2C_Queue_Enqueue(i2c_queue_t* queue, i2c_req_t* req)
{
	if (I2C_Queue_IsFull(queue))
		return false;
	
	queue->reqs[queue->head] = req;
	queue->head = (queue->head + 1) & (MAX_I2C_REQS - 1);
	queue->num_reqs++;
	
	return true;
}

bool I2C_Queue_Dequeue(i2c_queue_t* queue, i2c_req_t** req)
{
	if (I2C_Queue_IsEmpty(queue))
		return false;
	
	*req = queue->reqs[queue->tail];
	queue->tail = (queue->tail + 1) & (MAX_I2C_REQS - 1);
	queue->num_reqs--;
	
	return true;
}

bool I2C_Queue_Peek(const i2c_queue_t* queue, i2c_req_t** req)
{
	if (I2C_Queue_IsEmpty(queue))
		return false;
	
	*req = queue->reqs[queue->tail];
	return true;
}

bool I2C_Queue_IsEmpty(const i2c_queue_t* queue)
{
	return queue->num_reqs == 0;
}

bool I2C_Queue_IsFull(const i2c_queue_t* queue)
{
	return queue->num_reqs == MAX_I2C_REQS;
}
