#ifndef LED_H
#define LED_H

#include <stdbool.h>

#include "gpio.h"
#include "status.h"

typedef struct {
    gpio_port_t      port;
    gpio_pin_mask_t  pin;
    bool             polarity;
} led_t;

status_t led_init(led_t *led);
status_t led_on(led_t *led);
status_t led_off(led_t *led);
status_t led_toggle(led_t *led);
bool led_is_on(const led_t *led);


#endif /* LED_H */