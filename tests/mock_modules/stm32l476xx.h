#ifndef STM32L476RG_H
#define STM32L476RG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __I volatile const
#define __O volatile
#define __IO volatile
#define __IM volatile const
#define __OM volatile
#define __IOM volatile


typedef struct {
    __IOM uint32_t CTRL;
    __IOM uint32_t LOAD;
    __IOM uint32_t VAL;
    __IM uint32_t CALIB;
} SYSTICK_TypeDef;

typedef struct {
    __IO uint32_t MODER;      
    __IO uint32_t OTYPER;     
    __IO uint32_t OSPEEDR;     
    __IO uint32_t PUPDR;       
    __IO uint32_t IDR;         
    __IO uint32_t ODR;        
    __IO uint32_t BSRR;        
   // __IO uint32_t LCKR;        
    __IO uint32_t AFR[2];     
   // __IO uint32_t BRR;        
  //  __IO uint32_t ASCR;      
} GPIO_TypeDef;

typedef struct {
    __IOM uint32_t CR;
    __IOM uint32_t ICSCR;
    __IOM uint32_t CFGR;
    __IOM uint32_t PLLCFGR;
    __IOM uint32_t AHB1ENR;
    __IOM uint32_t AHB2ENR;
    __IOM uint32_t AHB3ENR;
    __IOM uint32_t APB1ENR1;
    __IOM uint32_t APB1ENR2;
    __IOM uint32_t APB1ENR3;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
} PWR_TypeDef;


typedef struct {
    volatile uint32_t ACR;
} FLASH_TypeDef;


// Mock Peripheral instances
extern GPIO_TypeDef         mock_GPIOA;
extern GPIO_TypeDef         mock_GPIOB;
extern GPIO_TypeDef         mock_GPIOC;
extern GPIO_TypeDef         mock_GPIOD;
extern GPIO_TypeDef         mock_GPIOE;
extern GPIO_TypeDef         mock_GPIOF;
extern RCC_TypeDef          mock_RCC;
extern PWR_TypeDef          mock_PWR;
extern FLASH_TypeDef        mock_FLASH;

// Override real CMSIS hardware addresses
#define GPIOA    (&mock_GPIOA)
#define GPIOB    (&mock_GPIOB)
#define GPIOC    (&mock_GPIOC)
#define GPIOD    (&mock_GPIOD)
#define GPIOE    (&mock_GPIOE)
#define GPIOF    (&mock_GPIOF)


#define RCC     (&mock_RCC)
#define PWR     (&mock_PWR)
#define FLASH   (&mock_FLASH)

// RCC CR
#define RCC_CR_MSION             (1U << 0)
#define RCC_CR_MSIRDY            (1U << 1)
#define RCC_CR_MSIRGSEL          (1U << 3)
#define RCC_CR_MSIRANGE_Pos      4U
#define RCC_CR_MSIRANGE_Msk      (0xFU << RCC_CR_MSIRANGE_Pos)
#define RCC_CR_HSION             (1U << 8)
#define RCC_CR_HSIRDY            (1U << 10)
#define RCC_CR_HSEON             (1U << 16)
#define RCC_CR_HSERDY            (1U << 17)
#define RCC_CR_HSEBYP            (1U << 18)
#define RCC_CR_PLLON             (1U << 24)
#define RCC_CR_PLLRDY            (1U << 25)


// RCC PLLCFGR
#define RCC_PLLCFGR_PLLSRC_Pos   0U
#define RCC_PLLCFGR_PLLSRC_Msk   (3U << RCC_PLLCFGR_PLLSRC_Pos)
#define RCC_PLLCFGR_PLLSRC_MSI   (1U << RCC_PLLCFGR_PLLSRC_Pos)
#define RCC_PLLCFGR_PLLSRC_HSI   (2U << RCC_PLLCFGR_PLLSRC_Pos)
#define RCC_PLLCFGR_PLLSRC_HSE   (3U << RCC_PLLCFGR_PLLSRC_Pos)
#define RCC_PLLCFGR_PLLM_Pos     4U
#define RCC_PLLCFGR_PLLM_Msk     (7U << RCC_PLLCFGR_PLLM_Pos)
#define RCC_PLLCFGR_PLLN_Pos     8U
#define RCC_PLLCFGR_PLLN_Msk     (0x7FU << RCC_PLLCFGR_PLLN_Pos)
#define RCC_PLLCFGR_PLLP_Pos     27U
#define RCC_PLLCFGR_PLLP_Msk     (1U << RCC_PLLCFGR_PLLP_Pos)
#define RCC_PLLCFGR_PLLQ_Pos     21U
#define RCC_PLLCFGR_PLLQ_Msk     (3U << RCC_PLLCFGR_PLLQ_Pos)
#define RCC_PLLCFGR_PLLR_Pos     25U
#define RCC_PLLCFGR_PLLR_Msk     (3U << RCC_PLLCFGR_PLLR_Pos)
#define RCC_PLLCFGR_PLLREN       (1U << 24)


// RCC CFGR
#define RCC_CFGR_SW_Pos          0U
#define RCC_CFGR_SW_Msk          (3U << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SW_MSI          (0U << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SW_HSI          (1U << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SW_HSE          (2U << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SW_PLL          (3U << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SWS_Pos         2U
#define RCC_CFGR_SWS_Msk         (3U << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS_MSI         (0U << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS_HSI         (1U << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS_HSE         (2U << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS_PLL         (3U << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_HPRE_Pos        4U
#define RCC_CFGR_HPRE_Msk        (0xFU << RCC_CFGR_HPRE_Pos)
#define RCC_CFGR_PPRE1_Pos       8U
#define RCC_CFGR_PPRE1_Msk       (7U << RCC_CFGR_PPRE1_Pos)
#define RCC_CFGR_PPRE2_Pos       11U
#define RCC_CFGR_PPRE2_Msk       (7U << RCC_CFGR_PPRE2_Pos)

#define RCC_APB1ENR1_PWREN       (1U << 28)


#define PWR_CR1_VOS_Pos          9U
#define PWR_CR1_VOS_Msk          (3U << PWR_CR1_VOS_Pos)
#define PWR_CR1_VOS_0            (1U << PWR_CR1_VOS_Pos)
#define PWR_SR2_VOSF             (1U << 10)

#define FLASH_ACR_LATENCY_Pos    0U
#define FLASH_ACR_LATENCY_Msk    (7U << FLASH_ACR_LATENCY_Pos)

#define FLASH_ACR_PRFTEN         (1U << 8)
#define FLASH_ACR_ICEN           (1U << 9)
#define FLASH_ACR_DCEN           (1U << 10)

/* GPIO MODER */

#define GPIO_MODER_MODE0_Pos      0U
#define GPIO_MODER_MODE0_Msk      (0x3U << GPIO_MODER_MODE0_Pos)
#define GPIO_MODER_MODE1_Pos      2U
#define GPIO_MODER_MODE1_Msk      (0x3U << GPIO_MODER_MODE1_Pos)

/* GPIO OTYPER */
#define GPIO_OTYPER_OT0           (1U << 0)
#define GPIO_OTYPER_OT1           (1U << 1)

/* GPIO OSPEEDR */
#define GPIO_OSPEEDR_OSPEED0_Pos  0U
#define GPIO_OSPEEDR_OSPEED0_Msk  (0x3U << GPIO_OSPEEDR_OSPEED0_Pos)

/* GPIO PUPDR */
#define GPIO_PUPDR_PUPD0_Pos      0U
#define GPIO_PUPDR_PUPD0_Msk      (0x3U << GPIO_PUPDR_PUPD0_Pos)

/* GPIO IDR */
#define GPIO_IDR_ID0              (1U << 0)
#define GPIO_IDR_ID1              (1U << 1)

/* GPIO ODR */
#define GPIO_ODR_OD0              (1U << 0)
#define GPIO_ODR_OD1              (1U << 1)

/* GPIO BSRR */
#define GPIO_BSRR_BS0             (1U << 0)
#define GPIO_BSRR_BS1             (1U << 1)
#define GPIO_BSRR_BR0             (1U << 16)
#define GPIO_BSRR_BR1             (1U << 17)

/* GPIO BRR */
#define GPIO_BRR_BR0              (1U << 0)
#define GPIO_BRR_BR1              (1U << 1)


void stm32_mock_reset(void);
void stm32_mock_set_ready_flags(void);
void stm32_mock_clear_ready_flags(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32L476RG_H */