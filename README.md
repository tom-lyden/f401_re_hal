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

## Design

The project separates MCU-specific register handling from application-level hardware definitions.

The HAL exposes MCU capabilities such as GPIO configuration and access, while board-specific concepts such as LEDs, buttons and peripherals are intended to live in a separate board/circuit layer.

Peripheral register blocks are represented directly from the STM32 memory map.

## Target

* MCU: STM32F401RE
* Core: ARM Cortex-M4
* Language: C
* Toolchain: GNU Arm Embedded (`arm-none-eabi-gcc`)
* Build system: CMake

## Status

GPIO is the first completed peripheral abstraction.

Planned next steps include:

* Delay / timing abstraction
* Additional peripheral abstractions as required by future projects
* Using the HAL as the foundation for progressively more complex STM32 applications
