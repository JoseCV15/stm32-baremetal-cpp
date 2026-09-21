#pragma once

#include "stm32l476xx.h"
#include "cstdint"

namespace gpio
{
    enum class Mode: uint8_t
    {
        Input = 0x00U,
        Output = 0x01U,
        Alternate = 0x02U,
        analog = 0x03U
    };

    enum class Pull : uint8_t
    {
        None,
        Up,
        Down
    };

    enum class OutputType : uint8_t
    {
        PushPull,
        OpenDrain
    };

    enum class Speed : uint8_t
    {
        Low,
        Medium,
        High,
        VeryHigh
    };

    enum class State : uint8_t
    {
        Low,
        High
    };
}


class Gpio
{
public:
    Gpio(GPIO_TypeDef* port, uint8_t pin);

    void init(
        gpio::Mode mode, 
        gpio::Pull pull = gpio::Pull::None, 
        gpio::OutputType type = gpio::OutputType::PushPull, 
        gpio::Speed speed = gpio::Speed::Low,
        std::uint8_t af = 0U
    );

    /** @brief Sets a GPIO pin to a high state */
    void setPin();

    /** @brief Resets a GPIO pin to a low state */
    void resetPin();

    /** @brief Toggles the state of a GPIO pin */
    void togglePin();

    /* Optional: Writes the state of a GPIO pin*/
    void writePin(gpio::State state);

    /** @brief Reads the state of a GPIO pin */
    gpio::State readPin() const;

    /**
     * @brif Sets the mode for a GPIO pin
     * @param mode Output, input, alternate, analog
     */
    void setMode(gpio::Mode mode);

    /**
     * @brief Sets the pull-up/pull-down register for a GPIO pin
     * @param pull None, up, down
     */
    void setPull(gpio::Pull pull);

    /** @brief Configures the AFR for a given pin.
     *  @param af The alternate function number (0-15)
     */
    void setAlternate(std::uint8_t af);

    /**
     * @brief Sets the output speed for a GPIO pin
     * @param speed Low, medium, high, or very high
     */
    void setSpeed(gpio::Speed speed);

    /**
     * @brief Sets the output type of a GPIO pin
     * @param type Push-pull or open-drain
     */
    void setOutputType(gpio::OutputType type);


private:
    GPIO_TypeDef* m_port;
    uint8_t m_pin;
    
    static void enableClock(GPIO_TypeDef* port);
    static void disableClock(GPIO_TypeDef* port);

};