#include "led.h"
#include "systick.h"
#include "button.h"
#include "uart.h"


static led_t led_green = {
    .port = GPIO_PORT_A,
    .pin = GPIO_PIN_5,
    .polarity = true
};


extern "C" int main()
{
    uart_init();



    while (1)
    {

        uart_send_string(UART_ID_2, "TEST\r\n");

        for (volatile uint32_t i = 0; i < 1000000U; i++) {
            __asm volatile ("nop");
        }
        
    }
}