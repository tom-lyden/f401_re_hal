//
// Created by tomly on 08/09/2026.
//

#include <stm32f401re_interrupts.h>
#include <stm32f401re_handlers.h>

#include <stdint.h>

#define NUM_CORE_EXCEPTIONS (16)
#define IRQ_VECTOR_INDEX(irq) ((irq) + (NUM_CORE_EXCEPTIONS))

int main(void);
static void Reset_Handler(void);
static void Default_Handler(void);

extern uint32_t _stack_top[];
extern uint32_t _data_loadaddr[];

extern uint32_t _data[];
extern uint32_t _edata[];
extern uint32_t _bss[];
extern uint32_t _ebss[];

__attribute__ ((section(".isr_vector")))
__attribute__ ((used)) // Force emission of this object despite not being used in this file
const uint32_t isr_vector[] =
{
	(uint32_t)_stack_top,
	(uint32_t)Reset_Handler,
	(uint32_t)NMI_Handler,
	(uint32_t)HardFault_Handler,
	(uint32_t)MMFault_Handler,
	(uint32_t)BusFault_Handler,
	(uint32_t)UsageFault_Handler,
	(uint32_t)0, // Reserved
	(uint32_t)0, // Reserved
	(uint32_t)0, // Reserved
	(uint32_t)0, // Reserved
	(uint32_t)SVCall_Handler,
	(uint32_t)DebugMon_Handler,
	(uint32_t)0, // Reserved
	(uint32_t)PendSV_Handler,
	(uint32_t)SysTick_Handler,

	[IRQ_VECTOR_INDEX(IRQ_WWDG)]               = (uint32_t)WindowWatchdog_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI16)]             = (uint32_t)EXTI16_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI21)]             = (uint32_t)EXTI21_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI22)]             = (uint32_t)EXTI22_Handler,
	[IRQ_VECTOR_INDEX(IRQ_FLASH)]              = (uint32_t)Flash_Handler,
	[IRQ_VECTOR_INDEX(IRQ_RCC)]                = (uint32_t)RCC_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI0)]              = (uint32_t)EXTI0_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI1)]              = (uint32_t)EXTI1_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI2)]              = (uint32_t)EXTI2_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI3)]              = (uint32_t)EXTI3_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI4)]              = (uint32_t)EXTI4_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA1_STREAM0)]       = (uint32_t)DMA1_Stream0_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA1_STREAM1)]       = (uint32_t)DMA1_Stream1_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA1_STREAM2)]       = (uint32_t)DMA1_Stream2_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA1_STREAM3)]       = (uint32_t)DMA1_Stream3_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA1_STREAM4)]       = (uint32_t)DMA1_Stream4_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA1_STREAM5)]       = (uint32_t)DMA1_Stream5_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA1_STREAM6)]       = (uint32_t)DMA1_Stream6_Handler,
	[IRQ_VECTOR_INDEX(IRQ_ADC)]                = (uint32_t)ADC_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI9_5)]            = (uint32_t)EXTI9_5_Handler,
	[IRQ_VECTOR_INDEX(IRQ_TIM1_BRK_TIM9)]      = (uint32_t)TIM1_BRK_TIM9_Handler,
	[IRQ_VECTOR_INDEX(IRQ_TIM1_UP_TIM10)]      = (uint32_t)TIM1_UP_TIM10_Handler,
	[IRQ_VECTOR_INDEX(IRQ_TIM1_TRG_COM_TIM11)] = (uint32_t)TIM1_TRG_COM_TIM11_Handler,
	[IRQ_VECTOR_INDEX(IRQ_TIM1_CC)]            = (uint32_t)TIM1_CC_Handler,
	[IRQ_VECTOR_INDEX(IRQ_TIM2)]               = (uint32_t)TIM2_Handler,
	[IRQ_VECTOR_INDEX(IRQ_TIM3)]               = (uint32_t)TIM3_Handler,
	[IRQ_VECTOR_INDEX(IRQ_TIM4)]               = (uint32_t)TIM4_Handler,
	[IRQ_VECTOR_INDEX(IRQ_I2C1_EV)]            = (uint32_t)I2C1_EV_Handler,
	[IRQ_VECTOR_INDEX(IRQ_I2C1_ER)]            = (uint32_t)I2C1_ER_Handler,
	[IRQ_VECTOR_INDEX(IRQ_I2C2_EV)]            = (uint32_t)I2C2_EV_Handler,
	[IRQ_VECTOR_INDEX(IRQ_I2C2_ER)]            = (uint32_t)I2C2_ER_Handler,
	[IRQ_VECTOR_INDEX(IRQ_SPI1)]               = (uint32_t)SPI1_Handler,
	[IRQ_VECTOR_INDEX(IRQ_SPI2)]               = (uint32_t)SPI2_Handler,
	[IRQ_VECTOR_INDEX(IRQ_USART1)]             = (uint32_t)USART1_Handler,
	[IRQ_VECTOR_INDEX(IRQ_USART2)]             = (uint32_t)USART2_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI15_10)]          = (uint32_t)EXTI15_10_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI17)]             = (uint32_t)EXTI17_Handler,
	[IRQ_VECTOR_INDEX(IRQ_EXTI18)]             = (uint32_t)EXTI18_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA1_STREAM7)]       = (uint32_t)DMA1_Stream7_Handler,
	[IRQ_VECTOR_INDEX(IRQ_SDIO)]               = (uint32_t)SDIO_Handler,
	[IRQ_VECTOR_INDEX(IRQ_TIM5)]               = (uint32_t)TIM5_Handler,
	[IRQ_VECTOR_INDEX(IRQ_SPI3)]               = (uint32_t)SPI3_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA2_STREAM0)]       = (uint32_t)DMA2_Stream0_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA2_STREAM1)]       = (uint32_t)DMA2_Stream1_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA2_STREAM2)]       = (uint32_t)DMA2_Stream2_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA2_STREAM3)]       = (uint32_t)DMA2_Stream3_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA2_STREAM4)]       = (uint32_t)DMA2_Stream4_Handler,
	[IRQ_VECTOR_INDEX(IRQ_OTG_FS)]             = (uint32_t)OTG_FS_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA2_STREAM5)]       = (uint32_t)DMA2_Stream5_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA2_STREAM6)]       = (uint32_t)DMA2_Stream6_Handler,
	[IRQ_VECTOR_INDEX(IRQ_DMA2_STREAM7)]       = (uint32_t)DMA2_Stream7_Handler,
	[IRQ_VECTOR_INDEX(IRQ_USART6)]             = (uint32_t)USART6_Handler,
	[IRQ_VECTOR_INDEX(IRQ_I2C3_EV)]            = (uint32_t)I2C3_EV_Handler,
	[IRQ_VECTOR_INDEX(IRQ_I2C3_ER)]            = (uint32_t)I2C3_ER_Handler,
	[IRQ_VECTOR_INDEX(IRQ_FPU)]                = (uint32_t)FPU_Handler,
	[IRQ_VECTOR_INDEX(IRQ_SPI4)]               = (uint32_t)SPI4_Handler,
};

void Reset_Handler(void)
{
	uint32_t num_words = _edata - _data;
	for (int i = 0; i < num_words; i++)
	{
		_data[i] = _data_loadaddr[i];
	}

	num_words = _ebss - _bss;
	for (int i = 0; i < num_words; i++)
	{
		_bss[i] = 0;
	}

	main();

	while (1);
}

void Default_Handler(void)
{
	while (1);
}
