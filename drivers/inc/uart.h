#ifndef UART_H
#define UART_H

#include <stdint.h>

#include "stm32l476xx.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_ID_1 = 0,
    UART_ID_2,
    UART_ID_3,

    UART_ID_MAX
} uart_id_t;

typedef struct {
    uart_id_t uart_id;
    uint32_t baud_rate;
} uart_config_t;

status_t uart_init(void);
status_t uart_init_config(const uart_config_t *config);
status_t uart_deinit(uart_id_t uart_id);

status_t uart_send_byte(uart_id_t uart_id, uint8_t byte);
status_t uart_send_string(uart_id_t uart_id, const char *str);
status_t uart_receive_byte(uart_id_t uart_id, uint8_t *byte);
status_t uart_receive_string(uart_id_t uart_id, char *buffer_str, uint32_t buffer_size);

#ifdef __cplusplus
}
#endif



#endif // UART_H