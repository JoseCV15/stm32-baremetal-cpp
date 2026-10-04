#include "unity.h"
#include "gpio.h"
#include "stm32l476xx.h"

void setUp(void) 
{
    stm32_mock_reset();
}

void tearDown(void) {}


void test_gpio_write_high(void)
{
    status_t status = gpio_write_pin(GPIO_PORT_A, GPIO_PIN_5, true);
    TEST_ASSERT_EQUAL(STATUS_OK, status); 
}

void test_gpio_write_low(void)
{
    status_t status = gpio_write_pin(GPIO_PORT_A, GPIO_PIN_5, false);
    TEST_ASSERT_EQUAL(STATUS_OK, status);
}

void test_gpio_led(void)
{
    gpio_config_t config = { 
        .port = GPIO_PORT_A, 
        .pin_mask = GPIO_PIN_5, 
        .mode = GPIO_MODE_INPUT, 
        .otype = GPIO_OTYPE_PUSH_PULL, 
        .speed = GPIO_SPEED_LOW, 
        .pull = GPIO_PULL_UP 
    };
    status_t status = gpio_init(&config);
    TEST_ASSERT_EQUAL(STATUS_OK, status);

}



int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_gpio_write_high);
    RUN_TEST(test_gpio_write_low);
    RUN_TEST(test_gpio_led);


    return UNITY_END();
}