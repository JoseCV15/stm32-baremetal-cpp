#include "stm32l476xx.h"


GPIO_TypeDef mock_GPIOA;
GPIO_TypeDef mock_GPIOB;
GPIO_TypeDef mock_GPIOC;
GPIO_TypeDef mock_GPIOD;
GPIO_TypeDef mock_GPIOE;
GPIO_TypeDef mock_GPIOF;
RCC_TypeDef mock_RCC;
PWR_TypeDef mock_PWR;
FLASH_TypeDef mock_FLASH;


void stm32_mock_reset(void)
{
    mock_RCC.CR = 0U;
    mock_RCC.ICSCR = 0U;
    mock_RCC.CFGR = 0U;
    mock_RCC.PLLCFGR = 0U;
    mock_RCC.APB1ENR1 = 0U;

    mock_PWR.CR1 = 0U;
    mock_PWR.CR2 = 0U;
    mock_PWR.CR3 = 0U;
    mock_PWR.SR1 = 0U;
    mock_PWR.SR2 = 0U;

    mock_FLASH.ACR = 0U;

    mock_GPIOA = (GPIO_TypeDef){0};
    mock_GPIOB = (GPIO_TypeDef){0};
    mock_GPIOC = (GPIO_TypeDef){0};
    mock_GPIOD = (GPIO_TypeDef){0};
    mock_GPIOE = (GPIO_TypeDef){0};
    mock_GPIOF = (GPIO_TypeDef){0};

    
}


void stm32_mock_set_ready_flags(void)
{
    mock_RCC.CR |=
        RCC_CR_MSIRDY |
        RCC_CR_HSIRDY |
        RCC_CR_HSERDY |
        RCC_CR_PLLRDY;

    mock_PWR.SR2 &= ~PWR_SR2_VOSF;
}


void stm32_mock_clear_ready_flags(void)
{
    mock_RCC.CR &=
        ~(RCC_CR_MSIRDY |
          RCC_CR_HSIRDY |
          RCC_CR_HSERDY |
          RCC_CR_PLLRDY);
}