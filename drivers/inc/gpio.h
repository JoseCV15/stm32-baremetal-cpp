#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

#include "stm32l476xx.h"
#include "status.h"

/**
 * gpio ports
 */
typedef enum {
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,

    //...
    GPIO_PORT_MAX
} gpio_port_t;

// Gpio Af
typedef enum {
    GPIO_AF_0  = 0,
    GPIO_AF_1  = 1,
    GPIO_AF_2  = 2,
    GPIO_AF_3  = 3,
    GPIO_AF_4  = 4,
    GPIO_AF_5  = 5,
    GPIO_AF_6  = 6,
    GPIO_AF_7  = 7,
    GPIO_AF_8  = 8,
    GPIO_AF_9  = 9,
    GPIO_AF_10 = 10,
    GPIO_AF_11 = 11,
    GPIO_AF_12 = 12,
    GPIO_AF_13 = 13,
    GPIO_AF_14 = 14,
    GPIO_AF_15 = 15
} gpio_af_t;

// Gpio pins
typedef uint16_t gpio_pin_mask_t;
#define GPIO_PIN(pin)          ((gpio_pin_mask_t)(1U << (pin)))
#define GPIO_PIN_0             GPIO_PIN(0)
#define GPIO_PIN_1             GPIO_PIN(1)
#define GPIO_PIN_2             GPIO_PIN(2)
#define GPIO_PIN_3             GPIO_PIN(3)
#define GPIO_PIN_4             GPIO_PIN(4)
#define GPIO_PIN_5             GPIO_PIN(5)
#define GPIO_PIN_6             GPIO_PIN(6)
#define GPIO_PIN_7             GPIO_PIN(7)
#define GPIO_PIN_8             GPIO_PIN(8)
#define GPIO_PIN_9             GPIO_PIN(9)
#define GPIO_PIN_10            GPIO_PIN(10)
#define GPIO_PIN_11            GPIO_PIN(11)
#define GPIO_PIN_12            GPIO_PIN(12)
#define GPIO_PIN_13            GPIO_PIN(13)
#define GPIO_PIN_14            GPIO_PIN(14)
#define GPIO_PIN_15            GPIO_PIN(15)
/**
 * Gpio Modes
 */
typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALT_FUNC,
    GPIO_MODE_ANALOG
} gpio_mode_t;

/**
 * Gpio pull-up/pull-down
 */
typedef enum {
    GPIO_PULL_NONE,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
} gpio_pupd_t;

/**
 * Gpio output speed
 */
typedef enum {
    GPIO_SPEED_LOW,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_HIGH,
    GPIO_SPEED_VERY_HIGH
} gpio_speed_t;

/**
 * Gpio output type (push-pull / open drain)
 */
typedef enum {
    GPIO_OTYPE_PUSH_PULL,
    GPIO_OTYPE_OPEN_DRAIN
} gpio_otype_t;


typedef struct {
    gpio_port_t port;
    gpio_pin_mask_t pin_mask;
    gpio_mode_t mode;
    gpio_otype_t otype;
    gpio_pupd_t pull;
    gpio_speed_t speed;
    gpio_af_t af;
} gpio_config_t;

/**
 * @brief Initialize Gpio pins
 * @param config Gpio configuration
 * @return STATUS_OK on success or error code on failure
 */
status_t gpio_init(const gpio_config_t *config);

//void gpio_deinit(void);

/**
 * @brief Set pins to High state
 * @param port The Gpio port
 * @param pin_mask The pins mask
 * @param STATUS_OK on success or error code on failure
 */
status_t gpio_set_pin(gpio_port_t port, gpio_pin_mask_t pin_mask);

/**
 * @brief Clear Gpio pins to low state
 * @param port The Gpio port
 * @param pin_mask The pins mask
 * @param STATUS_OK on success or error code on failure
 */
status_t gpio_clear_pin(gpio_port_t port, gpio_pin_mask_t pin_mask);


/**
 * @brief Toggle gpio
 * @param port The Gpio port
 * @param pin_mask The pins mask
 * @param STATUS_OK on success or error code on failure
 */
status_t gpio_toggle_pin(gpio_port_t port, gpio_pin_mask_t pin_mask);

/**
 * @brief Write a state to a Gpio pins
 * @param port The Gpio port
 * @param pin_mask The pins mask
 * @param STATUS_OK on success or error code on failure
 */
status_t gpio_write_pin(gpio_port_t port, gpio_pin_mask_t pin_mask, bool state);

/**
 * @brief Reads the state of a Gpio pin
 * @param port The Gpio port
 * @param STATUS_OK on success or error code on failure
 */
uint8_t gpio_read_pin(gpio_port_t port, gpio_pin_mask_t pin_mask);


#endif