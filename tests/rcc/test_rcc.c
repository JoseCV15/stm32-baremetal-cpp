#include "unity.h"
#include "rcc.h"
#include "rcc_clock.h"
#include "stm32l476xx.h"

void setUp(void) 
{
    stm32_mock_reset();
}
void tearDown(void) {}

void test_rcc_init_rejects_hse(void)
{
    status_t status;
    status = rcc_init(RCC_SYSCLK_HSE, 80000000U);
    TEST_ASSERT_EQUAL(STATUS_INVALID_ARGUMENT, status);
}

void test_rcc_init_config_rejects_null(void)
{
    status_t status;
    status = rcc_init_config(NULL);
    TEST_ASSERT_EQUAL(STATUS_INVALID_ARGUMENT, status);
}

void test_rcc_init_config_rejects_zero_frequency(void)
{
    rcc_config_t config = {
        .src = RCC_SYSCLK_HSI16,
        .msi = RCC_MSI_4MHZ,
        .hse_mode = RCC_HSE_CRYSTAL,
        .hse_freq = 0U,
        .pll.src = RCC_PLL_SRC_HSI16,
        .target_sysclk_hz = 0U
    };

    status_t status;
    status = rcc_init_config(&config);

    TEST_ASSERT_EQUAL(STATUS_INVALID_ARGUMENT, status);
}

void test_rcc_init_config_rejects_frequency_above_max(void)
{
    rcc_config_t config = {
        .src = RCC_SYSCLK_HSI16,
        .msi = RCC_MSI_4MHZ,
        .hse_mode = RCC_HSE_CRYSTAL,
        .hse_freq = 0U,
        .pll.src = RCC_PLL_SRC_HSI16,
        .target_sysclk_hz = 80000001U
    };

    status_t status;
    status = rcc_init_config(&config);

    TEST_ASSERT_EQUAL(STATUS_INVALID_ARGUMENT, status);
}

void test_rcc_default_clock_state(void)
{
    TEST_ASSERT_EQUAL_UINT32(16000000U, rcc_get_sysclk());
    TEST_ASSERT_EQUAL_UINT32(16000000U, rcc_get_hclk());
    TEST_ASSERT_EQUAL_UINT32(16000000U, rcc_get_apb1_clk());
    TEST_ASSERT_EQUAL_UINT32(16000000U, rcc_get_apb2_clk());
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_rcc_init_rejects_hse);
    RUN_TEST(test_rcc_init_config_rejects_zero_frequency);

    RUN_TEST(test_rcc_init_config_rejects_null);
    RUN_TEST(test_rcc_init_config_rejects_frequency_above_max);
    RUN_TEST(test_rcc_default_clock_state);


    return UNITY_END();
}
