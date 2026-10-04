#include "stm32l476xx.h"
#include "gpio.h"
#include "led.h"

static led_t led_green = {
    .port = GPIO_PORT_A,
    .pin = GPIO_PIN_5,
    .polarity = true
};

extern "C" int main()
{

    led_init(&led_green);
    
    while (1)
    {
        led_toggle(&led_green);
    }
}