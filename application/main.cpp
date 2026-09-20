#include "stm32l476xx.h"

extern "C" int main()
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    GPIOA->MODER &= ~(3U << (5U * 2U));
    GPIOA->MODER |=  (1U << (5U * 2U));
    GPIOA->OTYPER &= ~(1U << 5U);
    GPIOA->OSPEEDR &= ~(3U << (5U * 2U));


    GPIOA->PUPDR &= ~(3U << (5U * 2U));

    while (true)
    {
        /*
         * Set PA5
         */
        GPIOA->BSRR = GPIO_BSRR_BS5;

        for (volatile uint32_t i = 0U; i < 100000U; ++i)
        {
        }


        /*
         * Reset PA5
         */
        GPIOA->BSRR = GPIO_BSRR_BR5;

        for (volatile uint32_t i = 0U; i < 100000U; ++i)
        {
        }
    }
}