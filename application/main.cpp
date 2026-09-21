#include "stm32l476xx.h"
#include "Gpio.hpp"

extern "C" int main()
{
    Gpio led(GPIOA, 5);
    Gpio button(GPIOC, 13);
    Gpio uartTx(GPIOA, 2);

    led.init(gpio::Mode::Output);
    button.init(gpio::Mode::Input, gpio::Pull::Up);
    uartTx.init(
        gpio::Mode::Alternate,
        gpio::Pull::None,
        gpio::OutputType::PushPull,
        gpio::Speed::VeryHigh,
        7U
    );

    while (true)
    {
        if (button.read() == gpio::State::Low) {
            led.set();
        }
        else {
            led.reset();
        }
    }
}