# STM32F401RE HAL

A small bare-metal hardware abstraction layer for the STM32F401RE, written in C.

The goal of this project is to build a minimal HAL directly from the STM32 reference manual and datasheet, while keeping hardware-specific details out of application code.

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
* Memory-mapped peripheral register abstractions

The GPIO implementation has been tested on real hardware using an STM32 Nucleo-F401RE and an external RGB LED.

### RCC

* GPIO peripheral clock enable
* HCLK frequency retrieval
* AHB prescaler handling
* HSI clock source support

### SysTick

* Configurable tick frequency
* AHB or AHB/8 clock source
* Optional interrupt and counter enable
* Monotonic 32-bit tick counter

The SysTick implementation has been tested on real hardware as the timing source for a delay implementation.

## Design

The project separates MCU-specific register handling from application-level hardware definitions.

The HAL exposes MCU capabilities such as GPIO, clock and system tick access, while board-specific concepts such as LEDs, buttons and peripherals are intended to live in a separate board/circuit layer.

Peripheral register blocks are represented directly from the STM32 memory map.

## Status

GPIO, basic RCC clock querying and SysTick timing are currently implemented and tested on hardware.

Planned next steps include:

* Additional peripheral abstractions as required by future projects
* Using the HAL as the foundation for progressively more complex STM32 applications
