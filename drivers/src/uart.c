#include "uart.h"
#include "gpio.h"
#include "rcc.h"

#include <stdint.h>
#include <stddef.h>

#define UART_DEBUG_ID           UART_ID_2
#define UART_DEBUG_BAUD_RATE    115200U

typedef struct {
    gpio_port_t port;
    gpio_pin_mask_t pin;
    gpio_af_t af;
} uart_config_pin_t;

typedef enum {
    UART_CLK_PLCK1 = 0,
    UART_CLK_PLCK2
} uart_clk_src_t;

typedef struct {
    USART_TypeDef *instance;
    uart_clk_src_t clk_src;
    uart_config_pin_t tx;
    uart_config_pin_t rx;
    // DMA
    // NVIC
} uart_hardware_t;

static const uart_hardware_t uart_hardware_table[UART_ID_MAX] = {
    [UART_ID_1] = {
        .instance = USART1,
        .clk_src = UART_CLK_PLCK2,
        .tx = {
            .port = GPIO_PORT_A,
            .pin = GPIO_PIN_9,
            .af = GPIO_AF_7
        },
        .rx = {
            .port = GPIO_PORT_A,
            .pin = GPIO_PIN_10,
            .af = GPIO_AF_7
        }
    },
    [UART_ID_2] = {
        .instance = USART2,
        .clk_src = UART_CLK_PLCK1,
        .tx = {
            .port = GPIO_PORT_A,
            .pin = GPIO_PIN_2,
            .af = GPIO_AF_7
        },
        .rx = {
            .port = GPIO_PORT_A,
            .pin = GPIO_PIN_3,
            .af = GPIO_AF_7
        }
    },
    [UART_ID_3] = {
        .instance = USART3,
        .clk_src = UART_CLK_PLCK1,
        .tx = {
            .port = GPIO_PORT_B,
            .pin = GPIO_PIN_10,
            .af = GPIO_AF_7
        },
        .rx = {
            .port = GPIO_PORT_B,
            .pin = GPIO_PIN_11,
            .af = GPIO_AF_7
        }
    }
};


static bool uart_is_valid(uart_id_t uart_id) { return uart_id < UART_ID_MAX ;}
static USART_TypeDef *uart_get_instance(uart_id_t uart_id) { return uart_hardware_table[uart_id].instance; }
static uint32_t uart_get_clock(uart_id_t uart_id)
{
    uint32_t clk = 0U;
    switch (uart_hardware_table[uart_id].clk_src) {
    case UART_CLK_PLCK1: clk = rcc_get_apb1_clk(); break;
    case UART_CLK_PLCK2: clk = rcc_get_apb2_clk(); break;
    default: break;
    }
    return clk;
}

static status_t uart_enable_clock(uart_id_t uart_id)
{
    status_t status = STATUS_INVALID_ARGUMENT;
    switch (uart_id) {
    case UART_ID_1:
        rcc_enable_clk_peripheral(RCC_PERIPHERAL_USART1);
        status = STATUS_OK;
        break;
    case UART_ID_2:
        rcc_enable_clk_peripheral(RCC_PERIPHERAL_USART2);
        status = STATUS_OK;
        break;
    case UART_ID_3:
        rcc_enable_clk_peripheral(RCC_PERIPHERAL_USART3);
        status = STATUS_OK;
        break;
    default:
        break;
    }
    return status;
}

static status_t uart_disable_clock(uart_id_t uart_id)
{
    status_t status = STATUS_INVALID_ARGUMENT;
    switch (uart_id) {
    case UART_ID_1:
        rcc_disable_clk_peripheral(RCC_PERIPHERAL_USART1);
        status = STATUS_OK;
        break;
    case UART_ID_2:
        rcc_disable_clk_peripheral(RCC_PERIPHERAL_USART2);
        status = STATUS_OK;
        break;
    case UART_ID_3:
        rcc_disable_clk_peripheral(RCC_PERIPHERAL_USART3);
        status = STATUS_OK;
        break;
    default:
        break;
    }
    return status;
}

static status_t uart_configure_gpio(uart_id_t uart_id)
{
    gpio_config_t config = {0U};
    status_t status;

    // TX
    config.port = uart_hardware_table[uart_id].tx.port;
    config.pin_mask = uart_hardware_table[uart_id].tx.pin;
    config.mode = GPIO_MODE_ALT_FUNC;
    config.otype = GPIO_OTYPE_PUSH_PULL;
    config.speed = GPIO_SPEED_HIGH;
    config.pull = GPIO_PULL_UP;
    config.af = uart_hardware_table[uart_id].tx.af;

    status = gpio_init(&config);
    if (status != STATUS_OK) {
        return status;
    }

    // Rx
    config.port = uart_hardware_table[uart_id].rx.port;
    config.pin_mask = uart_hardware_table[uart_id].rx.pin;
    config.pull = GPIO_PULL_UP;
    config.af = uart_hardware_table[uart_id].rx.af;

    status = gpio_init(&config);
    
    return status;
}

static uint16_t calc_baud_rate(uint32_t clock, uint32_t baudrate)
{
    return (uint16_t)((clock + (baudrate /2U)) / baudrate);
}


static status_t uart_configure_peripheral(uart_id_t uart_id, uint32_t baud_rate)
{
    uint32_t clock;
    USART_TypeDef *uart;

    uart = uart_get_instance(uart_id);
    clock = uart_get_clock(uart_id);
    // Disable uart
    uart->CR1 = 0U;
    uart->CR2 = 0U;
    uart->CR3 = 0U;

    // Oversmapling by 16
    uart->BRR = (uint32_t)calc_baud_rate(clock, baud_rate);
    // Enable uart
    uart->CR1 = USART_CR1_TE_Msk | USART_CR1_RE_Msk | USART_CR1_UE_Msk;
    return STATUS_OK;
}


status_t uart_init(void)
{
    static const uart_config_t default_config = {
        .uart_id = UART_DEBUG_ID,
        .baud_rate = UART_DEBUG_BAUD_RATE
    };
    return uart_init_config(&default_config);
}

status_t uart_init_config(const uart_config_t *config)
{
    status_t status;
    if (!config || !uart_is_valid(config->uart_id)) {
        return STATUS_INVALID_ARGUMENT;
    }
    // Enable clock
    status = uart_enable_clock(config->uart_id);
    if (status != STATUS_OK) return status;
    // Configure GPIO
    status = uart_configure_gpio(config->uart_id);
    if (status != STATUS_OK) return status;

    // Configure USART
    status = uart_configure_peripheral(config->uart_id, config->baud_rate);

    return status;
}

status_t uart_deinit(uart_id_t uart_id)
{
    USART_TypeDef *uart;
    if (!uart_is_valid(uart_id)) {
        return STATUS_INVALID_ARGUMENT;
    }
    uart = uart_get_instance(uart_id);
    if (!uart) {
        return STATUS_INVALID_ARGUMENT;
    }

    uart->CR1 = 0U;
    uart->CR2 = 0U;
    uart->CR3 = 0U;

    return uart_disable_clock(uart_id);
}

status_t uart_send_byte(uart_id_t uart_id, uint8_t byte)
{
    USART_TypeDef *uart;
    if (!uart_is_valid(uart_id)) {
        return STATUS_INVALID_ARGUMENT;
    }
    uart = uart_get_instance(uart_id);
    while ((uart->ISR & USART_ISR_TXE_Msk) == 0U) {
    }
    uart->TDR = (uint32_t)byte;
    return STATUS_OK;
}

status_t uart_send_string(uart_id_t uart_id, const char *str)
{
    status_t status;
    if (!uart_is_valid(uart_id) || (!str)) {
        return STATUS_INVALID_ARGUMENT;
    }

    while (*str != '\0') {
        status = uart_send_byte(uart_id, (uint8_t)*str);
        if (status != STATUS_OK) {
            break;
        }
        str++;
    }
    return status;
}

status_t uart_receive_byte(uart_id_t uart_id, uint8_t *byte)
{
    USART_TypeDef *uart;
    if (!uart_is_valid(uart_id) || !byte) {
        return STATUS_INVALID_ARGUMENT;
    }
    uart = uart_get_instance(uart_id);

    while ((uart->ISR & USART_ISR_RXNE_Msk) == 0U) {
    }
    *byte = (uint8_t)(uart->RDR & 0xffU);
    return STATUS_OK;
}

status_t uart_receive_string(uart_id_t uart_id, char *buffer_str, uint32_t buffer_size)
{
    uint32_t index = 0U;
    uint8_t byte;
    status_t status = STATUS_OK;

    if (!uart_is_valid(uart_id) || !buffer_str || (buffer_size < 2U)) {
        return STATUS_INVALID_ARGUMENT;
    }
    
    while (index < (buffer_size -1U)) {
        status = uart_receive_byte(uart_id, &byte);
        if (status != STATUS_OK) {
            break;
        }
        if (byte == (uint8_t)'\n') {
            break;
        }
        
        buffer_str[index] = (char)byte;
        index++;
    }
    buffer_str[index] = '\0';
    return status;

}
