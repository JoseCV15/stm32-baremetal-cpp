#include <stdint.h>
#include <stm32l476xx.h>

extern uint32_t _estack;
/* .data */
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
/* .bss */
extern uint32_t _sbss;
extern uint32_t _ebss;
/* C++ static constructors */
typedef void (*init_func_t)(void);
extern init_func_t __init_array_start[];
extern init_func_t __init_array_end[];
extern init_func_t __preinit_array_start[];
extern init_func_t __preinit_array_end[];

#define WEAK_DEFAULT_HANDLER(handler) void handler(void) __attribute__((weak, alias("Default_Handler")))

extern int main(void);
void SystemInit(void);
typedef void (*Interrupt_Handler)(void);
void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMonitor_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void WWDG_IRQHandler(void);
void PVD_PVM_IRQHandler(void);
void TAMP_STAMP_IRQHandler(void);
void RTC_WKUP_IRQHandler(void);
void FLASH_IRQHandler(void);
void RCC_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void ADC1_2_IRQHandler(void);
void CAN1_TX_IRQHandler(void);
void CAN1_RX0_IRQHandler(void);
void CAN1_RX1_IRQHandler(void);
void CAN1_SCE_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void TIM1_BRK_TIM15_IRQHandler(void);
void TIM1_UP_TIM16_IRQHandler(void);
void TIM1_TRG_COM_TIM17_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void RTC_Alarm_IRQHandler(void);
void DFSDM1_FLT3_IRQHandler(void);
void TIM8_BRK_IRQHandler(void);
void TIM8_UP_IRQHandler(void);
void TIM8_TRG_COM_IRQHandler(void);
void TIM8_CC_IRQHandler(void);
void ADC3_IRQHandler(void);
void FMC_IRQHandler(void);
void SDMMC1_IRQHandler(void);
void TIM5_IRQHandler(void);
void SPI3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void TIM7_IRQHandler(void);
void DMA2_Channel1_IRQHandler(void);
void DMA2_Channel2_IRQHandler(void);
void DMA2_Channel3_IRQHandler(void);
void DMA2_Channel4_IRQHandler(void);
void DMA2_Channel5_IRQHandler(void);
void DFSDM1_FLT0_IRQHandler(void);
void DFSDM1_FLT1_IRQHandler(void);
void DFSDM1_FLT2_IRQHandler(void);
void COMP_IRQHandler(void);
void LPTIM1_IRQHandler(void);
void LPTIM2_IRQHandler(void);
void OTG_FS_IRQHandler(void);
void DMA2_Channel6_IRQHandler(void);
void DMA2_Channel7_IRQHandler(void);
void LPUART1_IRQHandler(void);
void QUADSPI_IRQHandler(void);
void I2C3_EV_IRQHandler(void);
void I2C3_ER_IRQHandler(void);
void SAI1_IRQHandler(void);
void SAI2_IRQHandler(void);
void SWPMI1_IRQHandler(void);
void TSC_IRQHandler(void);
void LCD_IRQHandler(void);
void RNG_IRQHandler(void);
void FPU_IRQHandler(void);


void Default_Handler(void)
{
    while (1)
    {
        (void)0;
    }
}



/* Cortex-M4 exceptions */
WEAK_DEFAULT_HANDLER(NMI_Handler);
WEAK_DEFAULT_HANDLER(HardFault_Handler);
WEAK_DEFAULT_HANDLER(MemManage_Handler);
WEAK_DEFAULT_HANDLER(BusFault_Handler);
WEAK_DEFAULT_HANDLER(UsageFault_Handler);
WEAK_DEFAULT_HANDLER(SVC_Handler);
WEAK_DEFAULT_HANDLER(DebugMonitor_Handler);
WEAK_DEFAULT_HANDLER(PendSV_Handler);
WEAK_DEFAULT_HANDLER(SysTick_Handler);

/* STM32L476RG interrupts */
WEAK_DEFAULT_HANDLER(WWDG_IRQHandler);
WEAK_DEFAULT_HANDLER(PVD_PVM_IRQHandler);
WEAK_DEFAULT_HANDLER(TAMP_STAMP_IRQHandler);
WEAK_DEFAULT_HANDLER(RTC_WKUP_IRQHandler);
WEAK_DEFAULT_HANDLER(FLASH_IRQHandler);
WEAK_DEFAULT_HANDLER(RCC_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI0_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI1_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI2_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI3_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI4_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel1_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel2_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel3_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel4_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel5_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel6_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA1_Channel7_IRQHandler);
WEAK_DEFAULT_HANDLER(ADC1_2_IRQHandler);
WEAK_DEFAULT_HANDLER(CAN1_TX_IRQHandler);
WEAK_DEFAULT_HANDLER(CAN1_RX0_IRQHandler);
WEAK_DEFAULT_HANDLER(CAN1_RX1_IRQHandler);
WEAK_DEFAULT_HANDLER(CAN1_SCE_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI9_5_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM1_BRK_TIM15_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM1_UP_TIM16_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM1_TRG_COM_TIM17_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM1_CC_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM2_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM3_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM4_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C1_EV_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C1_ER_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C2_EV_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C2_ER_IRQHandler);
WEAK_DEFAULT_HANDLER(SPI1_IRQHandler);
WEAK_DEFAULT_HANDLER(SPI2_IRQHandler);
WEAK_DEFAULT_HANDLER(USART1_IRQHandler);
WEAK_DEFAULT_HANDLER(USART2_IRQHandler);
WEAK_DEFAULT_HANDLER(USART3_IRQHandler);
WEAK_DEFAULT_HANDLER(EXTI15_10_IRQHandler);
WEAK_DEFAULT_HANDLER(RTC_Alarm_IRQHandler);
WEAK_DEFAULT_HANDLER(DFSDM1_FLT3_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM8_BRK_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM8_UP_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM8_TRG_COM_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM8_CC_IRQHandler);
WEAK_DEFAULT_HANDLER(ADC3_IRQHandler);
WEAK_DEFAULT_HANDLER(FMC_IRQHandler);
WEAK_DEFAULT_HANDLER(SDMMC1_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM5_IRQHandler);
WEAK_DEFAULT_HANDLER(SPI3_IRQHandler);
WEAK_DEFAULT_HANDLER(UART4_IRQHandler);
WEAK_DEFAULT_HANDLER(UART5_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM6_DAC_IRQHandler);
WEAK_DEFAULT_HANDLER(TIM7_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA2_Channel1_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA2_Channel2_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA2_Channel3_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA2_Channel4_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA2_Channel5_IRQHandler);
WEAK_DEFAULT_HANDLER(DFSDM1_FLT0_IRQHandler);
WEAK_DEFAULT_HANDLER(DFSDM1_FLT1_IRQHandler);
WEAK_DEFAULT_HANDLER(DFSDM1_FLT2_IRQHandler);
WEAK_DEFAULT_HANDLER(COMP_IRQHandler);
WEAK_DEFAULT_HANDLER(LPTIM1_IRQHandler);
WEAK_DEFAULT_HANDLER(LPTIM2_IRQHandler);
WEAK_DEFAULT_HANDLER(OTG_FS_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA2_Channel6_IRQHandler);
WEAK_DEFAULT_HANDLER(DMA2_Channel7_IRQHandler);
WEAK_DEFAULT_HANDLER(LPUART1_IRQHandler);
WEAK_DEFAULT_HANDLER(QUADSPI_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C3_EV_IRQHandler);
WEAK_DEFAULT_HANDLER(I2C3_ER_IRQHandler);
WEAK_DEFAULT_HANDLER(SAI1_IRQHandler);
WEAK_DEFAULT_HANDLER(SAI2_IRQHandler);
WEAK_DEFAULT_HANDLER(SWPMI1_IRQHandler);
WEAK_DEFAULT_HANDLER(TSC_IRQHandler);
WEAK_DEFAULT_HANDLER(LCD_IRQHandler);
WEAK_DEFAULT_HANDLER(RNG_IRQHandler);
WEAK_DEFAULT_HANDLER(FPU_IRQHandler);


/*
 * Copy initialized data from Flash to RAM
 */
static void copy_data(void)
{
    uint32_t *dst = &_sdata;
    uint32_t *src = &_sidata;

    while (dst < &_edata)
    {
        *dst++ = *src++;
    }
}


/*
 * Clear .bss
 */
static void zero_bss(void)
{
    uint32_t *dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst++ = 0U;
    }
}

/*
 * Initialize C++ static objects
 */
static void call_cpp_constructors(void)
{
    for (init_func_t *func = __preinit_array_start; func < __preinit_array_end; ++func){
        (*func)();
    }
    
    for (init_func_t *func = __init_array_start; func < __init_array_end;++func){
        (*func)();
    }
}

/*
 * Reset Handler
 */
void Reset_Handler(void)
{
    SCB->VTOR = 0x08008000U;
    __asm volatile ("dsb");
    __asm volatile ("isb");

    copy_data();
    zero_bss();
    call_cpp_constructors();

    SystemInit();
    main();

    while (1)
    {
        (void)0;
    }
}


/*
 * Vector Table
 */
__attribute__((section(".isr_vector"), used))
const uint32_t vector_table[] =
{
    /* Cortex-M4 core exceptions */
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
    (uint32_t)&BusFault_Handler,
    (uint32_t)&UsageFault_Handler,

    /* Reserved */
    0U,
    0U,
    0U,
    0U,

    (uint32_t)&SVC_Handler,
    (uint32_t)&DebugMonitor_Handler,

    /* Reserved */
    0U,

    (uint32_t)&PendSV_Handler,
    (uint32_t)&SysTick_Handler,


    /* STM32L476RG external interrupts */

    (uint32_t)&WWDG_IRQHandler,
    (uint32_t)&PVD_PVM_IRQHandler,
    (uint32_t)&TAMP_STAMP_IRQHandler,
    (uint32_t)&RTC_WKUP_IRQHandler,
    (uint32_t)&FLASH_IRQHandler,
    (uint32_t)&RCC_IRQHandler,
    (uint32_t)&EXTI0_IRQHandler,
    (uint32_t)&EXTI1_IRQHandler,
    (uint32_t)&EXTI2_IRQHandler,
    (uint32_t)&EXTI3_IRQHandler,
    (uint32_t)&EXTI4_IRQHandler,
    (uint32_t)&DMA1_Channel1_IRQHandler,
    (uint32_t)&DMA1_Channel2_IRQHandler,
    (uint32_t)&DMA1_Channel3_IRQHandler,
    (uint32_t)&DMA1_Channel4_IRQHandler,
    (uint32_t)&DMA1_Channel5_IRQHandler,
    (uint32_t)&DMA1_Channel6_IRQHandler,
    (uint32_t)&DMA1_Channel7_IRQHandler,
    (uint32_t)&ADC1_2_IRQHandler,
    (uint32_t)&CAN1_TX_IRQHandler,
    (uint32_t)&CAN1_RX0_IRQHandler,
    (uint32_t)&CAN1_RX1_IRQHandler,
    (uint32_t)&CAN1_SCE_IRQHandler,
    (uint32_t)&EXTI9_5_IRQHandler,
    (uint32_t)&TIM1_BRK_TIM15_IRQHandler,
    (uint32_t)&TIM1_UP_TIM16_IRQHandler,
    (uint32_t)&TIM1_TRG_COM_TIM17_IRQHandler,
    (uint32_t)&TIM1_CC_IRQHandler,
    (uint32_t)&TIM2_IRQHandler,
    (uint32_t)&TIM3_IRQHandler,
    (uint32_t)&TIM4_IRQHandler,
    (uint32_t)&I2C1_EV_IRQHandler,
    (uint32_t)&I2C1_ER_IRQHandler,
    (uint32_t)&I2C2_EV_IRQHandler,
    (uint32_t)&I2C2_ER_IRQHandler,
    (uint32_t)&SPI1_IRQHandler,
    (uint32_t)&SPI2_IRQHandler,
    (uint32_t)&USART1_IRQHandler,
    (uint32_t)&USART2_IRQHandler,
    (uint32_t)&USART3_IRQHandler,
    (uint32_t)&EXTI15_10_IRQHandler,
    (uint32_t)&RTC_Alarm_IRQHandler,
    (uint32_t)&DFSDM1_FLT3_IRQHandler,
    (uint32_t)&TIM8_BRK_IRQHandler,
    (uint32_t)&TIM8_UP_IRQHandler,
    (uint32_t)&TIM8_TRG_COM_IRQHandler,
    (uint32_t)&TIM8_CC_IRQHandler,
    (uint32_t)&ADC3_IRQHandler,
    (uint32_t)&FMC_IRQHandler,
    (uint32_t)&SDMMC1_IRQHandler,
    (uint32_t)&TIM5_IRQHandler,
    (uint32_t)&SPI3_IRQHandler,
    (uint32_t)&UART4_IRQHandler,
    (uint32_t)&UART5_IRQHandler,
    (uint32_t)&TIM6_DAC_IRQHandler,
    (uint32_t)&TIM7_IRQHandler,
    (uint32_t)&DMA2_Channel1_IRQHandler,
    (uint32_t)&DMA2_Channel2_IRQHandler,
    (uint32_t)&DMA2_Channel3_IRQHandler,
    (uint32_t)&DMA2_Channel4_IRQHandler,
    (uint32_t)&DMA2_Channel5_IRQHandler,
    (uint32_t)&DFSDM1_FLT0_IRQHandler,
    (uint32_t)&DFSDM1_FLT1_IRQHandler,
    (uint32_t)&DFSDM1_FLT2_IRQHandler,
    (uint32_t)&COMP_IRQHandler,
    (uint32_t)&LPTIM1_IRQHandler,
    (uint32_t)&LPTIM2_IRQHandler,
    (uint32_t)&OTG_FS_IRQHandler,
    (uint32_t)&DMA2_Channel6_IRQHandler,
    (uint32_t)&DMA2_Channel7_IRQHandler,
    (uint32_t)&LPUART1_IRQHandler,
    (uint32_t)&QUADSPI_IRQHandler,
    (uint32_t)&I2C3_EV_IRQHandler,
    (uint32_t)&I2C3_ER_IRQHandler,
    (uint32_t)&SAI1_IRQHandler,
    (uint32_t)&SAI2_IRQHandler,
    (uint32_t)&SWPMI1_IRQHandler,
    (uint32_t)&TSC_IRQHandler,
    (uint32_t)&LCD_IRQHandler,
    (uint32_t)&RNG_IRQHandler,
    (uint32_t)&FPU_IRQHandler
};