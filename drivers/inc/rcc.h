#ifndef RCC_H
#define RCC_H

#include <stdint.h>
#include <stdbool.h>

#include "rcc_clock.h"
#include "stm32l476xx.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Clock sources */
typedef enum {
    RCC_SYSCLK_MSI = 0,
    RCC_SYSCLK_HSI16,
    RCC_SYSCLK_HSE,
    RCC_SYSCLK_PLL
} rcc_clk_src_t;

typedef enum {
    RCC_PLL_SRC_MSI = 0,
    RCC_PLL_SRC_HSI16,
    RCC_PLL_SRC_HSE
} rcc_pll_src_t;


/* HSE */
typedef enum {
    RCC_HSE_CRYSTAL = 0,
    RCC_HSE_BYPASS
} rcc_hse_mode_t;

typedef enum {
    RCC_PERIPHERAL_GPIOA = 0U,
    RCC_PERIPHERAL_GPIOB,
    RCC_PERIPHERAL_GPIOC,
    RCC_PERIPHERAL_GPIOD,
    RCC_PERIPHERAL_GPIOE,
    RCC_PERIPHERAL_GPIOF,
    RCC_PERIPHERAL_GPIOG,
    RCC_PERIPHERAL_GPIOH,

    RCC_PERIPHERAL_USART1,
    RCC_PERIPHERAL_USART2,
    RCC_PERIPHERAL_USART3,

    RCC_PERIPHERAL_UART4,
    RCC_PERIPHERAL_UART5,

    RCC_PERIPHERAL_SPI1,
    RCC_PERIPHERAL_SPI2,
    RCC_PERIPHERAL_SPI3,

    RCC_PERIPHERAL_I2C1,
    RCC_PERIPHERAL_I2C2,
    RCC_PERIPHERAL_I2C3

} rcc_peripheral_t;

typedef struct {
    rcc_pll_src_t src;
} rcc_pll_config_t;

typedef struct {
    rcc_clk_src_t        src;
    rcc_msi_range_t      msi;
    rcc_hse_mode_t       hse_mode;
    uint32_t             hse_freq;
    rcc_pll_config_t     pll;
    uint32_t             target_sysclk_hz;
} rcc_config_t;

status_t rcc_init(rcc_clk_src_t src, uint32_t sysclk_hz);
status_t rcc_init_config(const rcc_config_t *config);

status_t rcc_enable_clk_peripheral(rcc_peripheral_t periheral);

uint32_t rcc_get_sysclk(void);
uint32_t rcc_get_hclk(void);
uint32_t rcc_get_apb1_clk(void);
uint32_t rcc_get_apb2_clk(void);


#ifdef __cplusplus
}
#endif

#endif /* RCC_H */