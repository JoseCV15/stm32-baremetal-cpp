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
    uart_send_string(UART_ID_2, "Hello STM32\r\n");


    while (1)
    {
        
    }
}