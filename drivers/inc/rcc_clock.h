#ifndef RCC_CLOCK_H
#define RCC_CLOCK_H

#include <stdbool.h>
#include <stdint.h>

#include "status.h"

#define RCC_PLL_INPUT_MIN_HZ    4000000U
#define RCC_PLL_INPUT_MAX_HZ    16000000U
#define RCC_PLL_VCO_MIN_HZ      64000000U
#define RCC_PLL_VCO_MAX_HZ      344000000U
#define RCC_PLL_OUTPUT_MIN_HZ   8000000U
#define RCC_PLL_OUTPUT_MAX_HZ   80000000U
#define RCC_SYSCLK_MAX_HZ       80000000U
#define RCC_APB_MAX_HZ          80000000U
#define RCC_HCLK_MAX_HZ         80000000U

typedef enum {
    RCC_MSI_100KHZ = 0U,
    RCC_MSI_200KHZ,
    RCC_MSI_400KHZ,
    RCC_MSI_800KHZ,
    RCC_MSI_1MHZ,
    RCC_MSI_2MHZ,
    RCC_MSI_4MHZ,
    RCC_MSI_8MHZ,
    RCC_MSI_16MHZ,
    RCC_MSI_24MHZ,
    RCC_MSI_32MHZ,
    RCC_MSI_48MHZ
} rcc_msi_range_t;

typedef struct {
    uint8_t m, n, r, q, p;
    uint32_t input_freq, vco_freq, output_freq;
} rcc_pll_factors_t;

typedef struct {
    uint32_t ahb_bits, apb1_bits, apb2_bits;
    uint16_t ahb_divider;
    uint8_t apb1_divider, apb2_divider;
} rcc_bus_factors_t;

uint32_t rcc_clock_msi_frequency(rcc_msi_range_t range);
status_t rcc_clock_pll_config(uint32_t source_frequency, uint32_t target_frequency, rcc_pll_factors_t *factors);
status_t rcc_clock_bus_config(uint32_t sysclk_frequency, rcc_bus_factors_t *factors);
uint32_t rcc_clock_flash_latency(uint32_t hclk_hz);


#endif /* RCC_CLOCK */