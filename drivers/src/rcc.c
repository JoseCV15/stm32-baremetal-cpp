#include "rcc.h"

#include <stddef.h>

#include "stm32l476xx.h"

// constants
#define RCC_TIMEOUT                   1000000U
#define RCC_HSI16_FREQ_HZ             16000000U
#define RCC_SYSCLK_DEFAULT_HZ         80000000U

// Clock state - represent the last successfully configured clock tree
// Initialize with safe frequency so that the getters return valid values even if RCC initialization fails.
static uint32_t s_sysclk       = RCC_HSI16_FREQ_HZ;
static uint32_t s_hclk         = RCC_HSI16_FREQ_HZ;
static uint32_t s_apb1_clk     = RCC_HSI16_FREQ_HZ;
static uint32_t s_apb2_clk     = RCC_HSI16_FREQ_HZ;

/**
 * @brief Wait for a register field to reach an expected value
 * @param reg The register pointer
 * @param mask The field mask
 * @param expected The expected value after masking
 * 
 * @return STATUS_OK on success or other error codes if not reached
*/
static status_t rcc_wait_flag(volatile uint32_t *reg, uint32_t mask, uint32_t expected)
{
    if (!reg) return STATUS_INVALID_ARGUMENT;
    for (uint32_t t = RCC_TIMEOUT; t > 0U; --t) {
            if ((*reg & mask) == expected) {
                return STATUS_OK;
            }
        }
        return STATUS_TIMEOUT;
}

/**
 * @brief Enable MSI
 */
static status_t rcc_enable_msi(rcc_msi_range_t msi)
{
    if (msi > RCC_MSI_48MHZ) {
        return STATUS_INVALID_ARGUMENT;
    }

    RCC->CR |= RCC_CR_MSIRGSEL;
    RCC->CR &= ~RCC_CR_MSIRANGE_Msk;

    RCC->CR |= ((uint32_t)msi << RCC_CR_MSIRANGE_Pos) & RCC_CR_MSIRANGE_Msk;
    RCC->CR |= RCC_CR_MSION;

    return rcc_wait_flag(&RCC->CR, RCC_CR_MSIRDY, RCC_CR_MSIRDY);

}

/**
 * @brief Enable HSI
 */
static status_t rcc_enable_hsi16(void) 
{
    RCC->CR |= RCC_CR_HSION;
    return rcc_wait_flag(&RCC->CR, RCC_CR_HSIRDY, RCC_CR_HSIRDY);
}

/**
 * @brief Enable HSE
 */
static status_t rcc_enable_hse(rcc_hse_mode_t hse)
{
    if (hse == RCC_HSE_BYPASS) {
        RCC->CR |= RCC_CR_HSEBYP;
    }
    else if (hse == RCC_HSE_CRYSTAL) {
        RCC->CR &= ~RCC_CR_HSEBYP;
    }
    else {
        return STATUS_INVALID_ARGUMENT;
    }

    RCC->CR |= RCC_CR_HSEON;
    return rcc_wait_flag(&RCC->CR, RCC_CR_HSERDY, RCC_CR_HSERDY);
}

/**
 * @brief Configure voltage scaling
 * Range 1 is required for the maximum system frequency
 */
static status_t rcc_configure_voltage_scale(void)
{
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    PWR->CR1 &= ~PWR_CR1_VOS_Msk;
    PWR->CR1 |= PWR_CR1_VOS_0;

    return rcc_wait_flag(&PWR->SR2, PWR_SR2_VOSF, 0U);
}

/**
 * @brief Configure Flash latency
 */
static status_t rcc_configure_flash(uint32_t hclk_hz)
{
    uint32_t latency;
    if (hclk_hz > RCC_SYSCLK_MAX_HZ) {
        return STATUS_INVALID_ARGUMENT;
    }
    latency = rcc_clock_flash_latency(hclk_hz);

    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY_Msk)
                 | latency
                 | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

    return rcc_wait_flag(&FLASH->ACR, FLASH_ACR_LATENCY_Msk, latency);
}

/**
 * @brief Get PLL input frequency from configuration
 */
static status_t rcc_get_pll_src_frequency(const rcc_config_t *config, uint32_t *frequency)
{
    if (!config || !frequency) {
        return STATUS_INVALID_ARGUMENT;
    }

    switch (config->pll.src)
    {
        case RCC_PLL_SRC_MSI:
            *frequency = rcc_clock_msi_frequency(config->msi);
            if (*frequency == 0U) {
                return STATUS_INVALID_ARGUMENT;
            }
            return STATUS_OK;

        case RCC_PLL_SRC_HSI16:
            *frequency = RCC_HSI16_FREQ_HZ;
            return STATUS_OK;

        case RCC_PLL_SRC_HSE:
            if (config->hse_freq == 0U) {
                return STATUS_INVALID_ARGUMENT;
            }
            *frequency = config->hse_freq;
            return STATUS_OK;
        default:
            return STATUS_INVALID_ARGUMENT;
    }
}

/**
 * @brief Configure PLL registers
 */
static status_t rcc_configure_pll(const rcc_pll_factors_t *factors, rcc_pll_src_t src)
{
    uint32_t pllcfgr;
    uint32_t pll_src;

    if (!factors) {
        return STATUS_INVALID_ARGUMENT;
    }

    switch (src)
    {
        case RCC_PLL_SRC_MSI:
            pll_src = RCC_PLLCFGR_PLLSRC_MSI;
            break;
        case RCC_PLL_SRC_HSI16:
            pll_src = RCC_PLLCFGR_PLLSRC_HSI;
            break;
        case RCC_PLL_SRC_HSE:
            pll_src = RCC_PLLCFGR_PLLSRC_HSE;
            break;
        default:
            return STATUS_INVALID_ARGUMENT;
    }

    // Disable Pll before changing its configuration
    RCC->CR &= ~RCC_CR_PLLON;
    if(rcc_wait_flag(&RCC->CR, RCC_CR_PLLRDY, 0U) != STATUS_OK) {
        return STATUS_TIMEOUT;
    }

    pllcfgr = RCC->PLLCFGR;
    pllcfgr &=~(RCC_PLLCFGR_PLLSRC_Msk | RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLN_Msk | 
                    RCC_PLLCFGR_PLLR_Msk | RCC_PLLCFGR_PLLQ_Msk | RCC_PLLCFGR_PLLP_Msk);
    pllcfgr |= pll_src;

    pllcfgr |= ((uint32_t)(factors->m - 1U) << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM_Msk;
    pllcfgr |= ((uint32_t)factors->n << RCC_PLLCFGR_PLLN_Pos) & RCC_PLLCFGR_PLLN_Msk;
    pllcfgr |= (((uint32_t)(factors->r / 2U) - 1U) << RCC_PLLCFGR_PLLR_Pos) & RCC_PLLCFGR_PLLR_Msk;
    pllcfgr |= (((uint32_t)(factors->q / 2U) - 1U) << RCC_PLLCFGR_PLLQ_Pos) & RCC_PLLCFGR_PLLQ_Msk;
    pllcfgr |= ((factors->p == 17U ? 1U : 0U) << RCC_PLLCFGR_PLLP_Pos) & RCC_PLLCFGR_PLLP_Msk;

    // Only use the R
    pllcfgr |= RCC_PLLCFGR_PLLREN;
    RCC->PLLCFGR = pllcfgr;
    RCC->CR |= RCC_CR_PLLON;

    return rcc_wait_flag(&RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY);
}

/**
 * @brief Configure AHB and APB prescalers
 */
static status_t rcc_configure_bus(const rcc_bus_factors_t *bus)
{
    uint32_t cfgr;
    if (!bus) {
        return STATUS_INVALID_ARGUMENT;
    }

    cfgr = RCC->CFGR;

    cfgr &= ~(RCC_CFGR_HPRE_Msk | RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk);
    cfgr |= (bus->ahb_bits << RCC_CFGR_HPRE_Pos) & RCC_CFGR_HPRE_Msk;
    cfgr |= (bus->apb1_bits << RCC_CFGR_PPRE1_Pos) & RCC_CFGR_PPRE1_Msk;
    cfgr |= (bus->apb2_bits << RCC_CFGR_PPRE2_Pos) & RCC_CFGR_PPRE2_Msk;

    RCC->CFGR =  cfgr;
    return STATUS_OK;
}

/**
 * @brief Switch SYSCLK source
 */
static status_t rcc_switch_system_clock(rcc_clk_src_t src)
{
    uint32_t sw, sws;

    switch(src) {
        case RCC_SYSCLK_MSI:
            sw = RCC_CFGR_SW_MSI;
            sws = RCC_CFGR_SWS_MSI;
            break;
        case RCC_SYSCLK_HSI16:
            sw = RCC_CFGR_SW_HSI;
            sws = RCC_CFGR_SWS_HSI;
            break;
        case RCC_SYSCLK_HSE:
            sw = RCC_CFGR_SW_HSE;
            sws = RCC_CFGR_SWS_HSE;
            break;
        case RCC_SYSCLK_PLL:
            sw = RCC_CFGR_SW_PLL;
            sws = RCC_CFGR_SWS_PLL;
            break;
        default:
            return STATUS_INVALID_ARGUMENT;
    }

    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | sw;

    return rcc_wait_flag(&RCC->CFGR, RCC_CFGR_SWS_Msk, sws);
}


/**
 * @brief Initialize PLL SYSCLK
 */
static status_t rcc_init_pll(const rcc_config_t *config, uint32_t *sysclk_frequency, rcc_bus_factors_t *bus)
{
    status_t status;
    uint32_t source_frequency, hclk_frequency;
    rcc_pll_factors_t pll;

    status = rcc_get_pll_src_frequency(config, &source_frequency);
    if (status != STATUS_OK) {
        return status;
    }
    
    // Enable PLL source.
    switch (config->pll.src) {
        case RCC_PLL_SRC_MSI:
            status = rcc_enable_msi(config->msi);
            break;

        case RCC_PLL_SRC_HSI16:
            status = rcc_enable_hsi16();
            break;

        case RCC_PLL_SRC_HSE:
            status = rcc_enable_hse(config->hse_mode);
            break;

        default:
            return STATUS_INVALID_ARGUMENT;
    }

    if (status != STATUS_OK){
        return status;
    }

    // Calculate M/N/R.
    status = rcc_clock_pll_config(source_frequency, config->target_sysclk_hz, &pll);
    if (status != STATUS_OK) {
        return status;
    }

    *sysclk_frequency = pll.output_freq;

    // Calculate AHB/APB divisors.
    status = rcc_clock_bus_config(*sysclk_frequency, bus);
    if (status != STATUS_OK)
    {
        return status;
    }

    // Flash latency must be ready before increasing HCLK.
    hclk_frequency = *sysclk_frequency / bus->ahb_divider;
    status = rcc_configure_flash(hclk_frequency);
    if (status != STATUS_OK) {
        return status;
    }

    // Configure PLL.
    status = rcc_configure_pll(&pll, config->pll.src);
    if (status != STATUS_OK) {
        return status;
    }

    // Configure bus dividers.
    status = rcc_configure_bus(bus);
    if (status != STATUS_OK) {
        return status;
    }

    return rcc_switch_system_clock(RCC_SYSCLK_PLL);
}
   


// Public API
/**
 * @brief Initialize RCC with a simple configuration
 * It supports MSI, HSI16, PLL (using hsi16 as pll src)
 * HSE requiresd rcc_init_config() as its frequency and mode must be explicitly specified
 */

status_t rcc_init(rcc_clk_src_t src, uint32_t sysclk_hz)
{
    rcc_config_t config = {
        .src = src,
        .msi = RCC_MSI_4MHZ,
        .hse_mode = RCC_HSE_CRYSTAL,
        .hse_freq = 0U,
        .pll.src = RCC_PLL_SRC_HSI16,
        .target_sysclk_hz = sysclk_hz
    };

    // HSE requires explicit frequency. Use rcc_init_config() when HSE is selected
    if (src == RCC_SYSCLK_HSE) {
        return STATUS_INVALID_ARGUMENT;
    }

    return rcc_init_config(&config);
}

/**
 * @brief Initialize RCC configuration
 */
status_t rcc_init_config(const rcc_config_t *config)
{
    status_t status;
    uint32_t hclk_freq, sysclk_freq;
    rcc_bus_factors_t bus;

    if (!config) {
        return STATUS_INVALID_ARGUMENT;
    }

    if ((config->target_sysclk_hz > RCC_SYSCLK_MAX_HZ) || (config->target_sysclk_hz == 0U)) {
        return STATUS_INVALID_ARGUMENT;
    }

    // Configure voltage scaling before increasing SYSCLK
    status = rcc_configure_voltage_scale();
    if (status != STATUS_OK) {
        return status;
    }

    // PLL SYSCLK
    if (config->src == RCC_SYSCLK_PLL) {
        /*
        * If PLL is already the current SYSCLK, switch to HSI16 before reconfiguring PLL.
        */
        status = rcc_enable_hsi16();
        if (status != STATUS_OK) {
            return status;
        }
        status = rcc_switch_system_clock(RCC_SYSCLK_HSI16);
        if (status != STATUS_OK) {
            return status;
        }

        status = rcc_init_pll(config, &sysclk_freq, &bus);
        if (status != STATUS_OK) {
            return status;
        }
    }

    else {
        switch (config->src) {
            case RCC_SYSCLK_MSI:
                status = rcc_enable_msi(config->msi);
                if (status != STATUS_OK) {
                    return status;
                }
                sysclk_freq = rcc_clock_msi_frequency(config->msi);
                break;
            case RCC_SYSCLK_HSI16:
                status = rcc_enable_hsi16();
                if (status != STATUS_OK) {
                    return status;
                }
                sysclk_freq = RCC_HSI16_FREQ_HZ;
                break;
            case RCC_SYSCLK_HSE:
                if (config->hse_freq == 0U) {
                    return STATUS_INVALID_ARGUMENT;
                }
                status = rcc_enable_hse(config->hse_mode);
                if (status != STATUS_OK) {
                    return status;
                }
                sysclk_freq = config->hse_freq;
                break;
            default:
                return STATUS_INVALID_ARGUMENT;
        }

        if ((config->target_sysclk_hz != sysclk_freq)) {
            return STATUS_INVALID_ARGUMENT;
        }

        status = rcc_clock_bus_config(sysclk_freq, &bus);
        if (status != STATUS_OK) {
            return status;
        }

        hclk_freq = sysclk_freq / bus.ahb_divider;

        status = rcc_configure_flash(hclk_freq);
        if (status != STATUS_OK) {
            return status;
        }

        status = rcc_configure_bus(&bus);
        if (status != STATUS_OK) {
            return status;
        }

        status = rcc_switch_system_clock(config->src);
        if (status != STATUS_OK){
            return status;
        }
    }

    // Update driver clock state
    s_sysclk = sysclk_freq;
    s_hclk = sysclk_freq / bus.ahb_divider;
    s_apb1_clk = s_hclk / bus.apb1_divider;
    s_apb2_clk = s_hclk / bus.apb2_divider;

    return STATUS_OK;

}

status_t rcc_enable_clk_peripheral(rcc_peripheral_t peripheral) {
    switch (peripheral) {
        case RCC_PERIPHERAL_GPIOA:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
            break;
        case RCC_PERIPHERAL_GPIOB:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
            break;
        case RCC_PERIPHERAL_GPIOC:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
            break;
        case RCC_PERIPHERAL_GPIOD:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
            break;
        case RCC_PERIPHERAL_GPIOE:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
            break;
        case RCC_PERIPHERAL_GPIOF:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
            break;
        case RCC_PERIPHERAL_GPIOG:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;
            break;
        case RCC_PERIPHERAL_GPIOH:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
            break;
        case RCC_PERIPHERAL_USART1:
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            break;
        case RCC_PERIPHERAL_USART2:
            RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
            break;
        case RCC_PERIPHERAL_USART3:
            RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN;
            break;
        case RCC_PERIPHERAL_UART4:
            RCC->APB1ENR1 |= RCC_APB1ENR1_UART4EN;
            break;
        case RCC_PERIPHERAL_UART5:
            RCC->APB1ENR1 |= RCC_APB1ENR1_UART5EN;
            break;
        case RCC_PERIPHERAL_SPI1:
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
            break;
        case RCC_PERIPHERAL_SPI2:
            RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
            break;
        case RCC_PERIPHERAL_SPI3:
            RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
            break;
        case RCC_PERIPHERAL_I2C1:
            RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
            break;
        case RCC_PERIPHERAL_I2C2:
            RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN;
            break;
        case RCC_PERIPHERAL_I2C3:
            RCC->APB1ENR1 |= RCC_APB1ENR1_I2C3EN;
            break;
        default:
            return STATUS_INVALID_ARGUMENT;
    }

    return STATUS_OK;
}


status_t rcc_disable_clk_peripheral(rcc_peripheral_t peripheral) {
    switch (peripheral) {
        case RCC_PERIPHERAL_GPIOA:
            RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOAEN;
            break;
        case RCC_PERIPHERAL_GPIOB:
            RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOBEN;
            break;
        case RCC_PERIPHERAL_GPIOC:
            RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOCEN;
            break;
        case RCC_PERIPHERAL_GPIOD:
            RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIODEN;
            break;
        case RCC_PERIPHERAL_GPIOE:
            RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOEEN;
            break;
        case RCC_PERIPHERAL_GPIOF:
            RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOFEN;
            break;
        case RCC_PERIPHERAL_GPIOG:
            RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOGEN;
            break;
        case RCC_PERIPHERAL_GPIOH:
            RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOHEN;
            break;
        case RCC_PERIPHERAL_USART1:
            RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
            break;
        case RCC_PERIPHERAL_USART2:
            RCC->APB1ENR1 &= ~RCC_APB1ENR1_USART2EN;
            break;
        case RCC_PERIPHERAL_USART3:
            RCC->APB1ENR1 &= ~RCC_APB1ENR1_USART3EN;
            break;
        case RCC_PERIPHERAL_UART4:
            RCC->APB1ENR1 &= ~RCC_APB1ENR1_UART4EN;
            break;
        case RCC_PERIPHERAL_UART5:
            RCC->APB1ENR1 &= ~RCC_APB1ENR1_UART5EN;
            break;
        case RCC_PERIPHERAL_SPI1:
            RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
            break;
        case RCC_PERIPHERAL_SPI2:
            RCC->APB1ENR1 &= ~RCC_APB1ENR1_SPI2EN;
            break;
        case RCC_PERIPHERAL_SPI3:
            RCC->APB1ENR1 &= ~RCC_APB1ENR1_SPI3EN;
            break;
        case RCC_PERIPHERAL_I2C1:
            RCC->APB1ENR1 &= ~RCC_APB1ENR1_I2C1EN;
            break;
        case RCC_PERIPHERAL_I2C2:
            RCC->APB1ENR1 &= ~RCC_APB1ENR1_I2C2EN;
            break;
        case RCC_PERIPHERAL_I2C3:
            RCC->APB1ENR1 &= ~RCC_APB1ENR1_I2C3EN;
            break;
        default:
            return STATUS_INVALID_ARGUMENT;
    }

    return STATUS_OK;
}


/**
 * CMSIS-standard entry point called from Reset_Handler before main()
 * Configures the system clock to 80 MHz using HSI16 as the PLL input source
 * Declared weak so host unit tests can provide am empty overrude without
 * causing a liner script duplicate-symbol error.
 */
__attribute__((weak)) void SystemInit(void)
{
    (void)rcc_init(RCC_SYSCLK_PLL, RCC_SYSCLK_DEFAULT_HZ);
}

uint32_t rcc_get_sysclk(void)     { return s_sysclk; }
uint32_t rcc_get_hclk(void)       { return s_hclk; }
uint32_t rcc_get_apb1_clk(void)   { return s_apb1_clk; }
uint32_t rcc_get_apb2_clk(void)   { return s_apb2_clk; }