#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

#include "stm32l476xx.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Initialize the Cortex-M SysTick Timer
* Configures sysTick to generate interrupts at the request frequency (e.g., 1ms)
* @param tick_hz Tick frequency
* @return STATUS_OK if succeeds or other error codes otherwise
*/
status_t systick_init(uint32_t tick_hz);

/**
* @brief Delay to wait a specific amount of time
* Blocks the CPU until the number of milliseconds has elapsed
* @param delay_ms Delay duration
*/
void systick_delay_ms(uint32_t delay_ms);

/**
* @brief Get the current Systick counter value
* The counter is incremented by the Systick interrupt handler.
* When the SysTick is configured for 1 kHz , the returned value
* represents elapsed milliseconds since intitialization.
* @return Current SysTick counter value
*/
uint32_t systick_get_tick(void);

#ifdef __cplusplus
}
#endif

#endif // SYSTICK_H