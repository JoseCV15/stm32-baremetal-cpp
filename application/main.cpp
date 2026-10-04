#include "led.h"
#include "systick.h"


static led_t led_green = {
    .port = GPIO_PORT_A,
    .pin = GPIO_PIN_5,
    .polarity = true
};


extern "C" int main()
{
    led_init(&led_green);

    if (systick_init(1000U) != STATUS_OK)
    {
        while (1)
        {
            led_on(&led_green);
        }
    }

    while (1)
    {
        led_toggle(&led_green);
        systick_delay_ms(500U);
    }
}