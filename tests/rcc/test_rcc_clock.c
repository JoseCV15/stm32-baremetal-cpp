#include "unity.h"
#include "rcc_clock.h"
#include "status.h"

void setUp(void)     {}
void tearDown(void)  {}


void test_msi_frequency_100khz(void)
{
    TEST_ASSERT_EQUAL_UINT32(100000U, rcc_clock_msi_frequency(RCC_MSI_100KHZ));
}

void test_msi_frequency_16Mhz_invalid(void)
{
    TEST_ASSERT_EQUAL_UINT32(16000000U, rcc_clock_msi_frequency(RCC_MSI_16MHZ));
}

// PLL
void test_pll_hsi16_to_80mhz(void)
{
    rcc_pll_factors_t pll;
    status_t status = rcc_clock_pll_config(16000000U, 80000000U, &pll);

    /*
    * m = 1, n = 10, r = 2
    */

    TEST_ASSERT_EQUAL(STATUS_OK, status);
    TEST_ASSERT_EQUAL_UINT8(1U, pll.m);
    TEST_ASSERT_EQUAL_UINT8(10U, pll.n);
    TEST_ASSERT_EQUAL_UINT8(2U, pll.q);

    TEST_ASSERT_EQUAL_UINT32(16000000U, pll.input_freq);
    TEST_ASSERT_EQUAL_UINT32(160000000U, pll.vco_freq);
    TEST_ASSERT_EQUAL_UINT32(80000000U, pll.output_freq);
}

void test_pll_8mhz_to_80mhz(void)
{
    rcc_pll_factors_t pll;
    status_t status = rcc_clock_pll_config(8000000U, 80000000U, &pll);
    TEST_ASSERT_EQUAL(STATUS_OK, status);

    TEST_ASSERT_EQUAL_UINT32(8000000U, pll.input_freq);
    TEST_ASSERT_EQUAL_UINT32(160000000U, pll.vco_freq);
    TEST_ASSERT_EQUAL_UINT32(80000000U, pll.output_freq);
}

void test_pll_rejects_input_freq_threshold(void)
{
    rcc_pll_factors_t pll;

    status_t status = rcc_clock_pll_config(1000000U, 80000000U, &pll);
    TEST_ASSERT_EQUAL(STATUS_INVALID_ARGUMENT, status);

    status = rcc_clock_pll_config(1000000U, 90000000U, &pll);
    TEST_ASSERT_EQUAL(STATUS_INVALID_ARGUMENT, status);
}

// Bus
void test_bus_80mhz(void)
{
    rcc_bus_factors_t bus;
    status_t status = rcc_clock_bus_config(80000000U, &bus);
    TEST_ASSERT_EQUAL(STATUS_OK, status);

    TEST_ASSERT_EQUAL_UINT16(1U, bus.ahb_divider);
    TEST_ASSERT_EQUAL_UINT8(1U, bus.apb1_divider);
    TEST_ASSERT_EQUAL_UINT8(1U, bus.apb2_divider);
}

// Flash
void test_flash_latency_returns_valid(void)
{
    uint32_t latency = rcc_clock_flash_latency(80000000U);
    TEST_ASSERT_NOT_EQUAL(UINT32_MAX, latency);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_msi_frequency_100khz);
    RUN_TEST(test_msi_frequency_16Mhz_invalid);

    RUN_TEST(test_pll_hsi16_to_80mhz);
    RUN_TEST(test_pll_8mhz_to_80mhz);
    RUN_TEST(test_pll_rejects_input_freq_threshold);

    RUN_TEST(test_bus_80mhz);

    RUN_TEST(test_flash_latency_returns_valid);


    return UNITY_END();
}