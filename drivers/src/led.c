#include "led.h"

status_t led_init(led_t *led)
{
    if (!led || led->pin == 0U){
        return STATUS_INVALID_ARGUMENT;
    }

    gpio_config_t config = {
        .port = led->port,
        .pin_mask = led->pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PUSH_PULL,
        .speed = GPIO_SPEED_LOW,
        .af = GPIO_AF_0
    };

    return gpio_init(&config);
}

status_t led_on(led_t *led)
{
    if (!led || led->pin == 0U){
        return STATUS_INVALID_ARGUMENT;
    }
    if (led->polarity) {
        return gpio_set_pin(led->port, led->pin);
    }
    else {
        return gpio_clear_pin(led->port, led->pin);
    }
}

status_t led_off(led_t *led)
{
    if (!led || led->pin == 0U){
        return STATUS_INVALID_ARGUMENT;
    }
    if (led->polarity) {
        return gpio_clear_pin(led->port, led->pin);
    }
    else {
        return gpio_set_pin(led->port, led->pin);
    }
}

status_t led_toggle(led_t *led)
{
    if (!led || led->pin == 0U){
        return STATUS_INVALID_ARGUMENT;
    }
    return gpio_toggle_pin(led->port, led->pin);
}

bool led_is_on(const led_t *led)
{
    gpio_pin_mask_t state;

    if (!led || led->pin == 0U)
        return STATUS_INVALID_ARGUMENT;
  
    state = gpio_read_pin(led->port, led->pin);
    if (led->polarity)
        return (state & led->pin) != 0U;
    else
        return (state & led->pin) == 0U;

}