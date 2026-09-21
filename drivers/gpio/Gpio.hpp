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
    void set();
    void reset();

    void write(gpio::State state);
    gpio::State read() const;
    void toggle();

    void setMode(gpio::Mode mode);
    void setPull(gpio::Pull pull);
    void setAlternate(std::uint8_t af);
    void setSpeed(gpio::Speed speed);
    void setOutputType(gpio::OutputType type);


private:
    GPIO_TypeDef* m_port;
    uint8_t m_pin;
    
    static void enableClock(GPIO_TypeDef* port);
    static void disableClock(GPIO_TypeDef* port);

};