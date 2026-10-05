#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Logical state of the button */
typedef enum {
    BUTTON_PRESSED,
    BUTTON_RELEASED
} button_state_t;

/*
* Button identification available in the project.
* Built-in user button of the board
* Can be added any external button
*/

typedef enum {
    BUTTON_USER = 0,
    BUTTON_EXT,

    BUTTON_MAX
} button_type_t;

/*
* @brief  that represents a button.
* @param port GPIO port where the button is connected
* @param pin GPIO pin used by the button
* @param pull Configuration of pull-up/down resistor
* @param active_low true if button pressed when GPIO = low and viseversa
* @param state Actual state of the button after reading
* @param previous_state Previous state
* @param last_change_tick Change in the signal detected used for debounce
*/
typedef struct {
    gpio_port_t port;
    gpio_pin_mask_t pin;
    gpio_pupd_t pull;
    bool active_low;
    button_state_t state, previous_state;
    uint32_t last_change_tick;
} button_t;

status_t button_init(button_type_t btn_type);
button_state_t button_read(button_type_t btn_type);
uint8_t button_is_pressed(button_type_t btn_type);
uint8_t button_was_pressed(button_type_t btn_type);

#ifdef __cplusplus
}
#endif


#endif // BUTTON_H