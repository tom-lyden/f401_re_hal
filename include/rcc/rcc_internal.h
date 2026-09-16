//
// Created by tomly on 02/09/2026.
//

#ifndef F401_RE_HAL_RCC_INTERNAL_H
#define F401_RE_HAL_RCC_INTERNAL_H

#include <stdint.h>

#define HSI_FREQ_HZ (16000000U)

#define RCC_BASE_ADDR (0x40023800U)
#define RCC_BASE ((volatile rcc_t *)(RCC_BASE_ADDR))

#define RCC_CFG_SWS_OFFSET (2)
#define RCC_CFG_SWS_WIDTH (2)
#define RCC_CFG_HPRE_OFFSET (4)
#define RCC_CFG_HPRE_WIDTH (4)

#define RCC_APB2EN_SYSCFG_OFFSET (14)

typedef enum
{
	SYSCLK_HSI = 0,
	SYSCLK_HSE = 1,
	SYSCLK_PLL = 2,
} sysclk_t;

typedef enum
{
	AHB_DIV_1 = 0,

	// No division for any number 1-7

	AHB_DIV_2   = 8,
	AHB_DIV_4   = 9,
	AHB_DIV_8   = 10,
	AHB_DIV_16  = 11,
	AHB_DIV_64  = 12,
	AHB_DIV_128 = 13,
	AHB_DIV_256 = 14,
	AHB_DIV_512 = 15,
} ahb_scaling_t;

typedef struct
{
	uint32_t CR;
	uint32_t PLLCFGR;
	uint32_t CFGR;
	uint32_t CIR;
	uint32_t AHB1RSTR;
	uint32_t AHB2RSTR;
	uint32_t reserved0[2];
	uint32_t APB1RSTR;
	uint32_t APB2RSTR;
	uint32_t reserved1[2];
	uint32_t AHB1ENR;
	uint32_t AHB2ENR;
	uint32_t reserved2[2];
	uint32_t APB1ENR;
	uint32_t APB2ENR;
	uint32_t reserved3[2];
	uint32_t AHB1LPENR;
	uint32_t AHB2LPENR;
	uint32_t reserved4[2];
	uint32_t APB1LPENR;
	uint32_t APB2LPENR;
	uint32_t reserved5[2];
	uint32_t BDCR;
	uint32_t CSR;
	uint32_t reserved6[2];
	uint32_t SSCGR;
	uint32_t PLLI2SCFGR;
	uint32_t reserved7;
	uint32_t DCKCFGR;
} rcc_t;

#endif // F401_RE_HAL_RCC_INTERNAL_H
