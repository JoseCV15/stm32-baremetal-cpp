#include "stm32l476xx.h"
#include "Gpio.hpp"
#include "Led.hpp"

extern "C" int main()
{
    Led led(GPIOA, 5, false);
    Gpio button(GPIOC, 13);
    Gpio uartTx(GPIOA, 2);

    led.init();
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
        if (button.readPin() == gpio::State::Low) {
            led.on();
        }
        else {
            led.off();
        }
    }
}