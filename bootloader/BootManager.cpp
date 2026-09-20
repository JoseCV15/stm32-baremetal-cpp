#include "BootManager.hpp"
#include "stm32l476xx.h"

namespace {
    constexpr std::uint32_t SramStart = 0x20000000U;
    constexpr std::uint32_t SramEnd = 0x20018000U;
    constexpr std::uint32_t FlashStart = 0x08000000U;
    constexpr std::uint32_t FlashEnd = 0x20000000U;
}

void BootManager::run()
{
    if(isApplicationValid()) {
        jumpToApplication();
    }

    while (true) {
        // No valid application found
        // stays forever
    }

}

bool BootManager::isApplicationValid() const
{

    const auto appStack = *reinterpret_cast<const volatile std::uint32_t*>(ApplicationStartAddress);
    const auto appResetHandler = *reinterpret_cast<const volatile std::uint32_t*>(ApplicationStartAddress +4U);

    // Check stack pointer
    if ((appStack < SramStart) || (appStack > SramEnd)) {
        return false;
    }

    // Reset Handler must be inside Flash
    if ((appResetHandler & 1U) == 0U) {
        return false;
    }
    const auto resetAddress = appResetHandler & ~1U;
    if ((resetAddress < FlashStart) || (resetAddress >= FlashEnd)) {
        return false;
    }

    return true;

}


bool BootManager::jumpToApplication() const
{
    const auto appStack = *reinterpret_cast<const volatile std::uint32_t*>(ApplicationStartAddress);
    const auto appResetHandler = *reinterpret_cast<const volatile std::uint32_t*>(ApplicationStartAddress +4U);

    __disable_irq();

    SysTick->CTRL = 0U;
    SysTick->LOAD = 0U;
    SysTick->VAL = 0U;

    SCB->VTOR = ApplicationStartAddress;

    __DSB();
    __ISB();

    __set_MSP(appStack);

    const auto appResetHandlerPtr = reinterpret_cast<void (*)(void)>(appResetHandler);
    appResetHandlerPtr();

    while (true)
    {
        /* code */
    }
    

}