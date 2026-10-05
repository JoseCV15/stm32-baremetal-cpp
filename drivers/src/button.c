#include "button.h"
#include "systick.h"

// Mechanical Bouncing 20 ticks 
#define BUTTON_DEBOUNCE_MS   20U
#define USER_BUTTON_PORT GPIO_PORT_C
#define USER_BUTTON_PIN GPIO_PIN_13

/*
* Table containing all button configuration. This way the app doesn't need to know
* which button is connected to which peripheral
*/
static button_t button_table[BUTTON_MAX] = {
    [BUTTON_USER] = {
        .port = USER_BUTTON_PORT,
        .pin = USER_BUTTON_PIN,
        .pull = GPIO_PULL_NONE,
        .active_low = true,
        .state = BUTTON_RELEASED,
        .previous_state = BUTTON_RELEASED,
        .last_change_tick = 0U
    },
    // Add other button configurations here
};

/*
* Helper to check if the requested button is valid
*/
static bool button_is_valid(button_type_t type) {
    return type < BUTTON_MAX;
}

status_t button_init(button_type_t btn_type)
{
    gpio_config_t config = {0U};
    if (!button_is_valid(btn_type)) return STATUS_INVALID_ARGUMENT;

    config.port = button_table[btn_type].port;
    config.pin_mask = button_table[btn_type].pin;
    config.mode = GPIO_MODE_INPUT;
    config.pull = button_table[btn_type].pull;

    status_t status = gpio_init(&config);
    if (status != STATUS_OK) {
        return status;
    }

    button_table[btn_type].state = BUTTON_RELEASED;
    button_table[btn_type].previous_state = BUTTON_RELEASED;
    button_table[btn_type].last_change_tick = systick_get_tick();

    return STATUS_OK;
}

button_state_t button_read(button_type_t btn_type)
{
    bool state;
    button_state_t button_state = BUTTON_RELEASED;

    if (button_is_valid(btn_type)) {
        state = gpio_read_pin(button_table[btn_type].port, button_table[btn_type].pin);
        if (button_table[btn_type].active_low) {
            if (state) {
                button_state = BUTTON_RELEASED;
            }
            else {
                button_state = BUTTON_PRESSED;
            }
        }
        else {
            if (state) {
                button_state = BUTTON_PRESSED;
            }
            else {
                button_state = BUTTON_RELEASED;
            }
        }
    }

    return button_state;
}

uint8_t button_is_pressed(button_type_t btn_type)
{
    return (button_read(btn_type) == BUTTON_PRESSED) ? 1U : 0U ;
}

uint8_t button_was_pressed(button_type_t btn_type)
{
    uint32_t now;
    button_state_t raw_state;
    uint8_t pressed = 0U;

    if (button_is_valid(btn_type)) {
        now = systick_get_tick();
        raw_state = button_read(btn_type);
        if (raw_state != button_table[btn_type].state) {
            button_table[btn_type].last_change_tick = now;
            button_table[btn_type].state = raw_state;
        }
        else if ((now - button_table[btn_type].last_change_tick) >= BUTTON_DEBOUNCE_MS) {
            if ((button_table[btn_type].previous_state == BUTTON_RELEASED) && 
                (button_table[btn_type].state == BUTTON_PRESSED)) {
                    pressed = 1U;
            }
            button_table[btn_type].previous_state =  button_table[btn_type].state;
        }
        else {
            // Debounce period is still active
        } 
    }
    else {
        // Invalid button type
    }

    return pressed;
}