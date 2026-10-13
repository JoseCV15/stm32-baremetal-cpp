#include "logger.h"

#include <stdarg.h>
#include <cstdio>
extern "C"
{
#include "uart.h"
}

namespace {
    constexpr uart_id_t LOGGER_UART = UART_ID_2;
    constexpr uint32_t MESSAGE_BUFFER_SIZE = 192U;
    constexpr uint32_t OUTPUT_BUFFER_SIZE = 256U;
}

LogLevel Logger::s_level = LogLevel::Info;
Logger::TimeProvider Logger::s_timeProvider = nullptr;

void Logger::init(LogLevel level)
{
    s_level = level;
}

void Logger::setLevel(LogLevel level)
{
    s_level = level;
}

LogLevel Logger::getLevel()
{
    return s_level;
}

void Logger::setTimeProvider(TimeProvider time)
{
    s_timeProvider = time;
}


const char* Logger::levelToString(LogLevel level)
{
    switch (level) {
    case LogLevel::Debug: return "DEB";
    case LogLevel::Info: return "INF";
    case LogLevel::Warn: return "WARN";
    case LogLevel::Error: return "ERR";
    case LogLevel::Verbose: return "VERB";
    default:                return "?";
    }
}

uint32_t Logger::getTimeMs()
{
    if (s_timeProvider == nullptr) {
        return 0U;
    }
    return s_timeProvider();
}

void Logger::print(LogLevel level, const char* log_src, const char* log_format, ...)
{
    char output[OUTPUT_BUFFER_SIZE];
    char message[MESSAGE_BUFFER_SIZE];

    if (log_src == nullptr || log_format == nullptr || level > s_level || level == LogLevel::None) {
        return;
    }

    va_list args;
    va_start(args, log_format);
    const int res = vsnprintf(message, sizeof(message), log_format, args);
    va_end(args);

    if (res < 0) {
        return;
    }

    // Get the current timestamp
    const uint32_t timestamp = getTimeMs();

    const int outputLength = snprintf(
        output, sizeof(output), "[%s] (%lu) %s: %s\r\n",
        levelToString(level),
        static_cast<unsigned long>(timestamp),
        log_src,
        message
    );

    if (outputLength <= 0 || (static_cast<uint32_t>(outputLength) >= sizeof(output))) {
        return;
    }

    (void)uart_send_string(LOGGER_UART, output);

}