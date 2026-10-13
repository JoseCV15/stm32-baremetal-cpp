
#include "systick.h"
#include "uart.h"
#include "logger.h"
#include "ringbuffer.h"

static constexpr const char * TAG = "UART_ECHO";
static constexpr const uint8_t rx_buffer_capacity = 128U;
static rb_t rx_rb;
static uint8_t rx_storage[rx_buffer_capacity];


int main()
{
    uart_init();
    systick_init(1000U);

    Logger::init(LogLevel::Debug);

    if (rb_init(&rx_rb, rx_storage, rx_buffer_capacity) != RB_OK) {
        LOG_ERR(TAG, "Ring buffer failed to initialize");
        while (1) {}
    }

    LOG_INF(TAG, "UART Ringbuffer echo started");


    while (1)
    {
        uint8_t rx = 0;
        uint32_t written = 0;
        if (uart_receive_byte(UART_ID_2, &rx) != STATUS_OK) {
            continue;
        }

        if (rb_write(&rx_rb, &rx, 1U, &written) != RB_OK) {
            LOG_INF(TAG, "ring buffer write error");
            continue;
        }

        if (written != 1U) {
            LOG_INF(TAG, "ring buffer full: byte dropped");
            continue;
        }


        uint8_t tx = 0U;
        uint32_t read_count = 0U;

        if (rb_read(&rx_rb, &tx, 1U, &read_count) != RB_OK) {
            LOG_INF(TAG, "ring buffer read error");
            continue;
        }

        if (read_count != 1U) {
            LOG_INF(TAG, "ring buffer empty");
            continue;
        }

         if (uart_send_byte(UART_ID_2, tx) != STATUS_OK) {
            LOG_INF(TAG, "UART TX error");
            continue;
        }

        LOG_DEB(TAG, "RX='%c' (0x%02X) -> TX='%c' (0x%02X)",
            rx, (unsigned int)rx, tx, (unsigned int)tx
        );
        
        
    }
}