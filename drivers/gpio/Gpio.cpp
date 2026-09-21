#include "Gpio.hpp"

// Helper
namespace
{
    std::uint32_t getClockMask(GPIO_TypeDef* port)
    {
        if (port == GPIOA) return RCC_AHB2ENR_GPIOAEN;
        if (port == GPIOB) return RCC_AHB2ENR_GPIOBEN;
        if (port == GPIOC) return RCC_AHB2ENR_GPIOCEN;
        if (port == GPIOD) return RCC_AHB2ENR_GPIODEN;
        if (port == GPIOE) return RCC_AHB2ENR_GPIOEEN;
        if (port == GPIOF) return RCC_AHB2ENR_GPIOFEN;
        if (port == GPIOG) return RCC_AHB2ENR_GPIOGEN;
        if (port == GPIOH) return RCC_AHB2ENR_GPIOHEN;

        return 0U;
    }
}

Gpio::Gpio(GPIO_TypeDef* port, uint8_t pin)
    : m_port(port), m_pin(pin)
{
}

void Gpio::enableClock(GPIO_TypeDef* port)
{
    const std::uint32_t mask = getClockMask(port);
    if(mask != 0) {
        RCC->AHB2ENR |= mask;
    }
    (void)RCC->AHB2ENR;
}

void Gpio::disableClock(GPIO_TypeDef* port)
{
    const std::uint32_t mask = getClockMask(port);
    if(mask != 0) {
        RCC->AHB2ENR &= ~mask;
    }
}

void Gpio::setMode(gpio::Mode mode)
{
    const std::uint32_t shift = m_pin * 2U;
    const std::uint32_t mask = 0x3U << shift;

    m_port->MODER &= ~mask;
    m_port->MODER |= (static_cast<std::uint32_t>(mode) & 0x3U) << shift;
}

void Gpio::setPull(gpio::Pull pull)
{
    const std::uint32_t shift = m_pin * 2U;
    const std::uint32_t mask = 0x3U << shift;

    m_port->PUPDR &= ~mask;
    m_port->PUPDR |= (static_cast<std::uint32_t>(pull) & 0x3U) << shift;

}

void Gpio::setAlternate(std::uint8_t af)
{
    const std::uint8_t afrIndex = m_pin / 8U;
    const std::uint32_t shift = (m_pin % 8U) * 4U;

    m_port->AFR[afrIndex] &= ~(0xFU << shift);
    m_port->AFR[afrIndex] |= (static_cast<std::uint32_t>(af) & 0xFU) << shift;
}

void Gpio::setSpeed(gpio::Speed speed)
{
    const std::uint32_t shift = m_pin * 2U;
    const std::uint32_t mask = 0x3U << shift;

    m_port->OSPEEDR &= ~mask;
    m_port->OSPEEDR |= (static_cast<std::uint32_t>(speed) & 0x3U) << shift;
}

void Gpio::setOutputType(gpio::OutputType type)
{
    if (type == gpio::OutputType::OpenDrain) {
        m_port->OTYPER |= (1U << m_pin);
    }
    else {
        m_port->OTYPER &= ~(1U << m_pin);
    }
}

void Gpio::init(gpio::Mode mode, gpio::Pull pull, gpio::OutputType type, gpio::Speed speed,std::uint8_t af)
{

    enableClock(m_port);
    setMode(mode);
    setPull(pull);
    setOutputType(type);
    setSpeed(speed);

    if (mode == gpio::Mode::Alternate) {
        setAlternate(af);
    }
}

void Gpio::set()
{
    m_port->BSRR |= (1U << m_pin);
}

void Gpio::reset()
{
    m_port->BSRR |= (1U << (m_pin + 16U));
}

void Gpio::write(gpio::State state)
{
    if (state == gpio::State::High) {
        set();
    }
    else {
        reset();
    }
}

gpio::State Gpio::read() const
{
    const std::uint32_t mask = 1U << m_pin;

    if ((m_port->IDR & mask) != 0) {
        return gpio::State::High;
    }
    else {
        return gpio::State::Low;
    }
}

void Gpio::toggle()
{
    // Read current gpio state
    const std::uint32_t mask = 1U << m_pin;
    if ((m_port->ODR & mask) != 0) {
        reset();
    }
    else {
        set();
    }
}