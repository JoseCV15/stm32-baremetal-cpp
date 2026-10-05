#include "led.h"
#include "systick.h"
#include "button.h"


static led_t led_green = {
    .port = GPIO_PORT_A,
    .pin = GPIO_PIN_5,
    .polarity = true
};


extern "C" int main()
{
    led_init(&led_green);
    button_init(BUTTON_USER);

    systick_init(1000);


    while (1)
    {
        if (button_was_pressed(BUTTON_USER) != 0U) {
            led_toggle(&led_green);
        }
    }
}