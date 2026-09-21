#include "Led.hpp"

Led::Led(GPIO_TypeDef* port, std::uint8_t pin, bool activeLow)
    : m_gpio(port, pin),
      m_activeLow(activeLow)
{
}

void Led::init()
{
    m_gpio.init(gpio::Mode::Output);
    off();
}

void Led::on()
{
    if(m_activeLow) {
        m_gpio.resetPin(); // Led works when GPIO is active Low
    }
    else {
        m_gpio.setPin();
    }
}

void Led::off()
{
    if(m_activeLow) {
        m_gpio.setPin();
    }
    else {
        m_gpio.resetPin();
    }
}

void Led::toggle()
{
    m_gpio.togglePin();
}

bool Led::isOn() const
{
    const gpio::State state = m_gpio.readPin();
    if (m_activeLow) {
        return state == gpio::State::Low;
    }
    else {
        return state == gpio::State::High;
    }
}