#include "rcc_clock.h"
#include "status.h"


uint32_t rcc_clock_msi_frequency(rcc_msi_range_t msi)
{
    static const uint32_t s_msi_frequencies[] = {
        100000U, 200000U, 400000U, 800000U,
        1000000, 2000000U, 4000000U, 8000000,
        16000000U, 24000000U, 32000000U, 48000000
    };

    if ((uint32_t)msi >= (sizeof(s_msi_frequencies) / sizeof(s_msi_frequencies[0]))) {
        return 0U;
    }

    return s_msi_frequencies[(uint32_t)msi];
}

status_t rcc_clock_pll_config(uint32_t source_frequency, uint32_t target_frequency, rcc_pll_factors_t *factors)
{
    static const uint8_t r_values[] = {2U, 4U, 6U, 8U};
    if (!factors) {
        return STATUS_INVALID_ARGUMENT;
    }

    if((source_frequency < RCC_PLL_INPUT_MIN_HZ) || (source_frequency > RCC_PLL_INPUT_MAX_HZ)) {
        return STATUS_INVALID_ARGUMENT;
    }

    if((target_frequency < RCC_PLL_OUTPUT_MIN_HZ) || (target_frequency > RCC_PLL_OUTPUT_MAX_HZ)) {
        return STATUS_INVALID_ARGUMENT;
    }

    for(uint8_t m = 1U; m <= 8U; ++m) {
        uint32_t pll_input = source_frequency / m;
        if ((source_frequency % m) != 0U) {
            continue;
        }
        if ((pll_input < RCC_PLL_INPUT_MIN_HZ) || (pll_input > RCC_PLL_INPUT_MAX_HZ)) {
            continue;
        }

        for (uint8_t n = 8U; n <= 86U; ++n){
            uint64_t vco = ((uint64_t)source_frequency * n) / m;

            if ((vco < RCC_PLL_VCO_MIN_HZ) || (vco > RCC_PLL_VCO_MAX_HZ)) {
                continue;
            }

            for (uint32_t r_index = 0U; r_index < (sizeof(r_values) / sizeof(r_values[0])); ++r_index) {
                uint8_t r = r_values[r_index];
                uint64_t denominator = (uint64_t)m *r;
                uint64_t numerator = (uint64_t)source_frequency * n;

                if (numerator != ((uint64_t)target_frequency * denominator)) {
                    continue;
                }
                uint64_t output = numerator / denominator;

                if ((output < RCC_PLL_OUTPUT_MIN_HZ) || (output > RCC_PLL_OUTPUT_MAX_HZ)) {
                    continue;
                }

                factors->m = m;
                factors->n = n;
                factors->r = r;

                // not used kept default for now
                factors->q = 2U;
                factors->p = 7U;

                factors->input_freq = pll_input;
                factors->vco_freq = (uint32_t)vco;
                factors->output_freq = (uint32_t)output;

                return STATUS_OK;
            }
        }
       
    }

    return STATUS_NOT_SUPPORTED;
}


status_t rcc_clock_bus_config(uint32_t sysclk_frequency, rcc_bus_factors_t *factors)
{
    static const struct { uint32_t bits; uint16_t divider;} ahb_table[] = {
        {0x0U, 1U}, {0x8U, 2U}, {0x9U, 4U}, {0xAU, 8U}, {0xBU, 16U},
        {0xCU, 64U}, {0xDU, 128U}, {0xEU, 256U}, {0xFU, 512U}
    };

    static const struct { uint32_t bits; uint8_t divider;} apb_table[] = {
        {0x0U, 1U}, {0x4U, 2U}, {0x5U, 4U}, {0x6U, 8U}, {0x7U, 16U}
    };

    if (!factors || (sysclk_frequency == 0U) || (sysclk_frequency > RCC_SYSCLK_MAX_HZ)) {
        return STATUS_INVALID_ARGUMENT;
    }

    // HCLK must not exceed the maximum allowed frequency

    for (uint32_t i = 0U; i < sizeof(ahb_table) / sizeof(ahb_table[0]); ++i) {
        if ((sysclk_frequency / ahb_table[i].divider) <= RCC_HCLK_MAX_HZ) {
            factors->ahb_bits = ahb_table[i].bits;
            factors->ahb_divider = ahb_table[i].divider;
            break;
        }
    }

    // APB1 
    for (uint32_t i = 0U; i < sizeof(apb_table) / sizeof(apb_table[0]); ++i) {
        if ((sysclk_frequency / apb_table[i].divider) <= RCC_APB_MAX_HZ) {
            factors->apb1_bits = apb_table[i].bits;
            factors->apb1_divider = apb_table[i].divider;
            break;
        }
    }
    // APB2
    for (uint32_t i = 0U; i < sizeof(apb_table) / sizeof(apb_table[0]); ++i) {
        if ((sysclk_frequency / apb_table[i].divider) <= RCC_APB_MAX_HZ) {
            factors->apb2_bits = apb_table[i].bits;
            factors->apb2_divider = apb_table[i].divider;
            break;
        }
    }

    return STATUS_OK;

}

uint32_t rcc_clock_flash_latency(uint32_t hclk_hz)
{   
    /**
     * Flash latency, voltage range 1
     * WS CPU cycle
     */
    static const uint32_t max_frequency[] = {16000000U, 32000000U, 48000000U, 80000000};
    for (uint32_t ws = 0U; ws < sizeof(max_frequency) / sizeof(max_frequency[0]); ++ws){
        if(hclk_hz <= max_frequency[ws]) {
            return ws;
        }
    }
    return UINT32_MAX;
}


