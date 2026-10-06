# STM32F401RE HAL

A small bare-metal hardware abstraction layer for the STM32F401RE, written in C.

The goal of this project is to build a minimal HAL directly from the STM32 reference manual and datasheet, while keeping
hardware-specific details out of application code.

## Current Features

### GPIO

* GPIO port clock enable through RCC
* Pin configuration:
    * Input
    * Output
    * Alternate function
    * Analog
* Push-pull and open-drain outputs
* Configurable output speed
* Pull-up / pull-down configuration
* Alternate-function selection
* GPIO read, write and toggle
* GPIO configuration locking
* Interrupt configuration through EXTI

### I2C

* Standard-mode and fast-mode clock configuration
* 7-bit and 10-bit device addressing
* Interrupt-driven write transactions
* Queued transaction handling
* Multi-byte address / prefix transmission
* Multi-byte data transmission
* ACK / NACK handling
* Event and error interrupt handling
* Transaction cleanup and recovery after failed transfers
* Automatic GPIO alternate-function and open-drain configuration
* Bus and alternate-function selection from configured SDA / SCL pins

### EXTI / SYSCFG

* GPIO-to-EXTI line routing through SYSCFG
* Rising-edge, falling-edge and dual-edge triggering
* EXTI interrupt masking
* Pending interrupt acknowledgement
* Per-line callback registration and dispatch
* Shared interrupt handling for EXTI5–9 and EXTI10–15

### NVIC

* Cortex-M4 interrupt enable support
* IRQ-to-register and bit mapping
* Integration with peripheral interrupt configuration

### RCC

* GPIO peripheral clock enable
* I2C peripheral clock enable
* SYSCFG peripheral clock enable
* HCLK and PCLK1 frequency retrieval
* AHB and APB1 prescaler handling
* HSI clock source support

### SysTick

* Configurable tick frequency
* AHB or AHB/8 clock source
* Optional interrupt and counter enable
* Monotonic 32-bit tick counter

### MMIO

* Reusable memory-mapped register field access
* Register masking and field manipulation utilities

## Hardware Testing

The HAL is tested on an STM32 Nucleo-F401RE.

Current hardware validation includes:

* GPIO input and output
* LED control
* SysTick-based timing
* GPIO edge detection through EXTI
* SYSCFG EXTI routing
* NVIC interrupt delivery
* EXTI callback dispatch
* I2C START and STOP generation
* 7-bit device addressing
* 10-bit address header transmission
* Multi-byte I2C writes
* ACK and NACK detection
* I2C error handling and transaction recovery
* I2C bus traffic verification using a logic analyzer
* Communication with an external I2C OLED display

## Design

The project separates MCU-specific hardware access from application-level behavior.

The HAL exposes hardware capabilities such as GPIO, interrupts, clock control, system timing and I2C communication.
Higher-level drivers and board-specific components are intended to build on top of these primitives rather than becoming
part of the HAL itself.

Peripheral and Cortex-M4 register blocks are accessed directly through their documented memory-mapped interfaces.

I2C transfers are interrupt-driven and handled through an internal transaction state machine. Requests may contain
address / prefix bytes followed by an arbitrary-length data buffer, while peripheral events and errors are handled
asynchronously through the corresponding I2C interrupts.

## Status

GPIO, RCC, SysTick, EXTI, SYSCFG, basic NVIC support and interrupt-driven I2C write transactions are currently
implemented and tested on hardware.

I2C read transactions are not currently implemented.

Additional peripheral abstractions will be added as required by future projects rather than implemented speculatively.

The HAL is intended to serve as the low-level foundation for progressively more complex STM32 applications.
