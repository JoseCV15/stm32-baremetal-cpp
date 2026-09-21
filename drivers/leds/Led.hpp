#pragma once

#include "Gpio.hpp"
#include <cstdint>


class Led
{
public:
    Led(GPIO_TypeDef* port, std::uint8_t pin, bool activeLow = false);
    void init();
    void on();
    void off();
    void toggle();
    bool isOn() const;

private:
    Gpio m_gpio;
    bool m_activeLow;

};