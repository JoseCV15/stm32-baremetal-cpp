#include "systick.h"
#include "rcc.h"

static volatile uint32_t s_tick_ms = 0U;


static void systick_start(uint32_t reload)
{
    SysTick->LOAD = reload; 
    SysTick->VAL = 0UL; 
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

static void systick_stop(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->LOAD = 0;
}

void SysTick_Handler(void)
{
    s_tick_ms++;
}


status_t systick_init(uint32_t tick_hz)
{
    uint32_t reload;
    if (tick_hz == 0U) {
        return STATUS_INVALID_ARGUMENT;
    }
    reload = (rcc_get_sysclk() / tick_hz ) - 1U;
    if (reload > SysTick_LOAD_RELOAD_Msk) { // 24bit
        return STATUS_INVALID_ARGUMENT;
    }
    systick_start(reload);

    return STATUS_OK;
}

void systick_delay_ms(uint32_t delay_ms)
{
    uint32_t now = systick_get_tick();
    while (systick_get_tick() - now < delay_ms) {
         __asm volatile ("nop");
    } 
}

uint32_t systick_get_tick(void) { return s_tick_ms; }
