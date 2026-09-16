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
* Interrupt configuration through EXTI

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
* Integration with EXTI interrupt configuration

### RCC

* GPIO peripheral clock enable
* SYSCFG peripheral clock enable
* HCLK frequency retrieval
* AHB prescaler handling
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

## Design

The project separates MCU-specific hardware access from application-level behavior.

The HAL exposes hardware capabilities such as GPIO, interrupts, clock control and system timing. Higher-level drivers and board-specific components are intended to build on top of these primitives rather than becoming part of the HAL itself.

Peripheral and Cortex-M4 register blocks are accessed directly through their documented memory-mapped interfaces.

## Status

GPIO, RCC, SysTick, EXTI, SYSCFG and basic NVIC interrupt enable support are currently implemented and tested on hardware.

Additional peripheral abstractions will be added as required by future projects rather than implemented speculatively.

The HAL is intended to serve as the low-level foundation for progressively more complex STM32 applications.
