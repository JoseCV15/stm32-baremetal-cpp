#include "gpio.h"
#include "stm32l476xx.h"
#include "rcc.h"

static bool gpio_is_valid_port(gpio_port_t port)
{
    return port < GPIO_PORT_MAX;
}

static bool gpio_pin_selected(gpio_pin_mask_t mask, uint8_t pin)
{
    return (mask & GPIO_PIN(pin)) != 0U;
}

/**
 * Get the instance for a Gpio port
 */
static GPIO_TypeDef* gpio_get_port_instance(gpio_port_t port)
{
    switch (port)
    {
        case GPIO_PORT_A: return GPIOA;
        case GPIO_PORT_B: return GPIOB;
        case GPIO_PORT_C: return GPIOC;
        case GPIO_PORT_D: return GPIOD;
        case GPIO_PORT_E: return GPIOE;
        case GPIO_PORT_F: return GPIOF;
        default: return 0;
    }

}

/**
 * Enable clock for a specific Gpio port
 */
static status_t gpio_enable_clock(gpio_port_t port)
{
    rcc_peripheral_t peripheral;
    if (!gpio_is_valid_port(port)) {
        return STATUS_INVALID_ARGUMENT;
    }
    switch (port) {
        case GPIO_PORT_A: peripheral = RCC_PERIPHERAL_GPIOA; break;
        case GPIO_PORT_B: peripheral = RCC_PERIPHERAL_GPIOB; break;
        case GPIO_PORT_C: peripheral = RCC_PERIPHERAL_GPIOC; break;
        case GPIO_PORT_D: peripheral = RCC_PERIPHERAL_GPIOD; break;
        case GPIO_PORT_E: peripheral = RCC_PERIPHERAL_GPIOE; break;
        case GPIO_PORT_F: peripheral = RCC_PERIPHERAL_GPIOF; break;
        default: return STATUS_INVALID_ARGUMENT;
    }
    return rcc_enable_clk_peripheral(peripheral);
}


/**
 * Disable clock for a specific Gpio port
*/
static status_t gpio_disable_clock(gpio_port_t port)
{
    rcc_peripheral_t peripheral;
    if (!gpio_is_valid_port(port)) {
        return STATUS_INVALID_ARGUMENT;
    }
    switch (port) {
        case GPIO_PORT_A: peripheral = RCC_PERIPHERAL_GPIOA; break;
        case GPIO_PORT_B: peripheral = RCC_PERIPHERAL_GPIOB; break;
        case GPIO_PORT_C: peripheral = RCC_PERIPHERAL_GPIOC; break;
        case GPIO_PORT_D: peripheral = RCC_PERIPHERAL_GPIOD; break;
        case GPIO_PORT_E: peripheral = RCC_PERIPHERAL_GPIOE; break;
        case GPIO_PORT_F: peripheral = RCC_PERIPHERAL_GPIOF; break;
        default: return STATUS_INVALID_ARGUMENT;
    }
    return rcc_disable_clk_peripheral(peripheral);
}

/**
 * Configure gpio for a specific mode
 * @param port The Gpio port
 * @param pin_mask The pins
 * @param mode The modes to be set
 * @return STATUS_OK  on success or other errors on failure
*/
static status_t gpio_configure_mode(gpio_port_t port, gpio_pin_mask_t pin_mask, gpio_mode_t mode)
{
    if(!gpio_is_valid_port(port) || (pin_mask == 0U) || (mode > GPIO_MODE_ANALOG)) {
        return STATUS_INVALID_ARGUMENT;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    if (!gpio_port) return STATUS_INVALID_ARGUMENT;

    for (uint8_t pin = 0U; pin < 16U; pin++){
        if (gpio_pin_selected(pin_mask, pin)) {
            uint32_t shift = pin * 2U;
            // Clear the bits for the pin
            gpio_port->MODER &= ~(0x3U << shift);
            // Set the new mode
            gpio_port->MODER |= ((uint32_t)mode << shift);
        }
    }
    return STATUS_OK;
}


/**
 * Configure gpio for a specific mode
 * @param port The Gpio port
 * @param pin_mask The pins
 * @param mode The modes to be set
 * @return STATUS_OK  on success or other errors on failure
*/
static status_t gpio_configure_otype(gpio_port_t port, gpio_pin_mask_t pin_mask, gpio_otype_t otype)
{
    if(!gpio_is_valid_port(port) || (pin_mask == 0U) || (otype > GPIO_OTYPE_OPEN_DRAIN)) {
        return STATUS_INVALID_ARGUMENT;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    if (!gpio_port) return STATUS_INVALID_ARGUMENT;

    for (uint8_t pin = 0U; pin < 16U; pin++){
        if (gpio_pin_selected(pin_mask, pin)) {
            gpio_port->OTYPER &= ~(0x1U << pin);
            gpio_port->OTYPER |= (((uint32_t)otype & 0x1U) << pin);
        }
    }
    return STATUS_OK;
}


/**
 * Configure gpio for a specific mode
 * @param port The Gpio port
 * @param pin_mask The pins
 * @param mode The modes to be set
 * @return STATUS_OK  on success or other errors on failure
*/
static status_t gpio_configure_pupd(gpio_port_t port, gpio_pin_mask_t pin_mask, gpio_pupd_t pull)
{
    
    if(!gpio_is_valid_port(port) || (pin_mask == 0U) || ( pull > GPIO_PULL_DOWN)) {
        return STATUS_INVALID_ARGUMENT;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    if (!gpio_port) return STATUS_INVALID_ARGUMENT;

    for (uint8_t pin = 0U; pin < 16U; pin++){
        if (gpio_pin_selected(pin_mask, pin)) {
            uint32_t shift = pin * 2U;
            gpio_port->PUPDR &= ~(0x3U << shift);
            // Set the new mode
            gpio_port->PUPDR |= ((uint32_t)pull << shift);
        }
    }
    return STATUS_OK;
}

static status_t gpio_configure_speed(gpio_port_t port, gpio_pin_mask_t pin_mask, gpio_speed_t speed)
{
    if(!gpio_is_valid_port(port) || (pin_mask == 0U) || ( speed > GPIO_SPEED_VERY_HIGH)) {
        return STATUS_INVALID_ARGUMENT;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    if (!gpio_port) return STATUS_INVALID_ARGUMENT;

    for (uint8_t pin = 0U; pin < 16U; pin++){
        if (gpio_pin_selected(pin_mask, pin)) {
            uint32_t shift = pin * 2U;
            gpio_port->OSPEEDR &= ~(0x3U << shift);
            // Set the new mode
            gpio_port->OSPEEDR |= ((uint32_t)speed << shift);
        }
    }
    return STATUS_OK;
}

static status_t gpio_configure_af(gpio_port_t port, gpio_pin_mask_t pin_mask, gpio_af_t af)
{
    if(!gpio_is_valid_port(port) || (pin_mask == 0U) || ( af > GPIO_AF_15)) {
        return STATUS_INVALID_ARGUMENT;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    if (!gpio_port) return STATUS_INVALID_ARGUMENT;

    for (uint8_t pin = 0U; pin < 16U; pin++) {
        if (gpio_pin_selected(pin_mask, pin)) {
            uint32_t shift, index;
            /*
            * Pins 0-7 -> AFR[0]
            * Pins 8-15 -> AFR[1]
             */
            index = pin / 8U; // >> 3
            shift = (pin % 8U) * 4U;

            gpio_port->AFR[index] &= ~(0xFU << shift);
            gpio_port->AFR[index] |= ((uint32_t)af << shift);
        }
    }

    return STATUS_OK;

}


/**
 * Initialize the gpio
 */
status_t gpio_init(const gpio_config_t *config)
{
    status_t res;
    if (!config || !gpio_is_valid_port(config->port) || (config->pin_mask == 0U)) {
        return STATUS_INVALID_ARGUMENT;
    }

    // Enable clock
    res = gpio_enable_clock(config->port);
    if (res != STATUS_OK) {
        return res;
    }

    res = gpio_configure_mode(config->port, config->pin_mask, config->mode);
    if (res != STATUS_OK) {
        return res;
    }

    switch (config->mode)
    {
        case GPIO_MODE_INPUT:
            res = gpio_configure_pupd(config->port, config->pin_mask, config->pull);
            break;
        case GPIO_MODE_OUTPUT:
            res = gpio_configure_pupd(config->port, config->pin_mask, config->pull);
            if (res != STATUS_OK) {
                return res;
            }
            res = gpio_configure_otype(config->port, config->pin_mask, config->otype);
            if (res != STATUS_OK) {
                return res;
            }
            res = gpio_configure_speed(config->port, config->pin_mask, config->speed);
            if (res != STATUS_OK) {
                return res;
            }
            break;
        case GPIO_MODE_ALT_FUNC:
            res = gpio_configure_pupd(config->port, config->pin_mask, config->pull);
            if (res != STATUS_OK) {
                return res;
            }
            res = gpio_configure_otype(config->port, config->pin_mask, config->otype);
            if (res != STATUS_OK) {
                return res;
            }
            res = gpio_configure_speed(config->port, config->pin_mask, config->speed);
            if (res != STATUS_OK) {
                return res;
            }

            res = gpio_configure_af(config->port, config->pin_mask, config->af);
            if (res != STATUS_OK) {
                return res;
            }
            break;
        case GPIO_MODE_ANALOG:
            break;
        default:
            return STATUS_INVALID_ARGUMENT;
    }
    return res;


}


status_t gpio_set_pin(gpio_port_t port, gpio_pin_mask_t pin_mask)
{
    if(!gpio_is_valid_port(port) || (pin_mask == 0U)) {
        return STATUS_INVALID_ARGUMENT;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    if (!gpio_port) return STATUS_INVALID_ARGUMENT;

    gpio_port->BSRR = (uint32_t) pin_mask;
    return STATUS_OK;
}

status_t gpio_clear_pin(gpio_port_t port, gpio_pin_mask_t pin_mask)
{
    if(!gpio_is_valid_port(port) || (pin_mask == 0U)) {
        return STATUS_INVALID_ARGUMENT;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    if (!gpio_port) return STATUS_INVALID_ARGUMENT;

    gpio_port->BSRR = (uint32_t) (pin_mask << 16U);
    return STATUS_OK;
}

status_t gpio_toggle_pin(gpio_port_t port, gpio_pin_mask_t pin_mask)
{
    if(!gpio_is_valid_port(port) || (pin_mask == 0U)) {
        return STATUS_INVALID_ARGUMENT;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    if (!gpio_port) return STATUS_INVALID_ARGUMENT;

    gpio_port->ODR ^= (uint32_t)pin_mask;

    return STATUS_OK;
}

status_t gpio_write_pin(gpio_port_t port, gpio_pin_mask_t pin_mask, bool state)
{
    if(!gpio_is_valid_port(port) || (pin_mask == 0U)) {
        return STATUS_INVALID_ARGUMENT;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    if (!gpio_port) return STATUS_INVALID_ARGUMENT;
    
    if (state) {
        return gpio_set_pin(port, pin_mask);
    }
    else {
        return gpio_clear_pin(port, pin_mask);
    }

}

uint8_t gpio_read_pin(gpio_port_t port, gpio_pin_mask_t pin_mask)
{
    if(!gpio_is_valid_port(port) || (pin_mask == 0U)) {
        return false;
    }
    GPIO_TypeDef* gpio_port = gpio_get_port_instance(port);
    
    return (((gpio_port->IDR) & pin_mask) ? 1U : 0U );
}
