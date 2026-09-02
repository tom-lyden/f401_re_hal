//
// Created by tomly on 02/09/2026.
//

#ifndef F401_RE_HAL_GPIO_H
#define F401_RE_HAL_GPIO_H

typedef enum
{
	GPIO_PORT_A = 0,
	GPIO_PORT_B = 1,
	GPIO_PORT_C = 2,
	GPIO_PORT_D = 3,
	
	// Reserved
	
	GPIO_PORT_H = 7,
} gpio_port_t;

typedef enum
{
	GPIO_PIN_0 = 0,
	GPIO_PIN_1 = 1,
	GPIO_PIN_2 = 2,
	GPIO_PIN_3 = 3,
	GPIO_PIN_4 = 4,
	GPIO_PIN_5 = 5,
	GPIO_PIN_6 = 6,
	GPIO_PIN_7 = 7,
	GPIO_PIN_8 = 8,
	GPIO_PIN_9 = 9,
	GPIO_PIN_10 = 10,
	GPIO_PIN_11 = 11,
	GPIO_PIN_12 = 12,
	GPIO_PIN_13 = 13,
	GPIO_PIN_14 = 14,
	GPIO_PIN_15 = 15,
} gpio_pin_t;

typedef enum
{
	GPIO_MODE_INPUT = 0,
	GPIO_MODE_OUTPUT = 1,
	GPIO_MODE_ALTERNATE = 2,
	GPIO_MODE_ANALOG = 3,
} gpio_mode_t;

typedef enum
{
	GPIO_TYPE_PUSH_PULL = 0,
	GPIO_TYPE_OPEN_DRAIN = 1,
} gpio_type_t;

typedef enum
{
	GPIO_SPEED_LOW = 0,
	GPIO_SPEED_MEDIUM = 1,
	GPIO_SPEED_HIGH = 2,
	GPIO_SPEED_VERY_HIGH = 3,
} gpio_speed_t;

typedef enum
{
	GPIO_PUPD_NONE = 0,
	GPIO_PUPD_PULLUP = 1,
	GPIO_PUPD_PULLDOWN = 2,
} gpio_pupd_t;

typedef enum
{
	GPIO_AF_0 = 0,
	GPIO_AF_1 = 1,
	GPIO_AF_2 = 2,
	GPIO_AF_3 = 3,
	GPIO_AF_4 = 4,
	GPIO_AF_5 = 5,
	GPIO_AF_6 = 6,
	GPIO_AF_7 = 7,
	GPIO_AF_8 = 8,
	GPIO_AF_9 = 9,
	GPIO_AF_10 = 10,
	GPIO_AF_11 = 11,
	GPIO_AF_12 = 12,
	GPIO_AF_13 = 13,
	GPIO_AF_14 = 14,
	GPIO_AF_15 = 15,
} gpio_af_t;

typedef enum
{
	GPIO_UNLOCKED = 0,
	GPIO_LOCKED = 1,
} gpio_lock_t;

typedef enum
{
	GPIO_STATE_LOW = 0,
	GPIO_STATE_HIGH = 1,
} gpio_state_t;

typedef struct
{
	gpio_mode_t mode;
	gpio_type_t type;
	gpio_speed_t speed;
	gpio_pupd_t pupd;
	gpio_af_t af;
	gpio_lock_t lock;
} gpio_config_t;

// Configuration
void GPIO_Init(gpio_port_t port, gpio_pin_t pin, const gpio_config_t* cfg);
void GPIO_Lock(gpio_port_t port, gpio_pin_t pin);

// Usage
void GPIO_Write(gpio_port_t port, gpio_pin_t pin, gpio_state_t state);
void GPIO_Toggle(gpio_port_t port, gpio_pin_t pin);
gpio_state_t GPIO_Read(gpio_port_t port, gpio_pin_t pin);

#endif // F401_RE_HAL_GPIO_H
