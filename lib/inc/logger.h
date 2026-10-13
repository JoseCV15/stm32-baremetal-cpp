#ifndef LOGGER_H
#define LOGGER_H

#include <cstdint>

/**
 * Support for multiple log levels
 * Verbose - All visible
 * Debug - diagnostic information (Debug, Info, Warn, Error)
 * Info - Normal system behavior  (Info, Warn, Error)
 * Warn - Unexpected but recoverable states (Warn, Error)
 * Error - Failures (Error)
*/
enum class LogLevel : uint8_t {
    None, Error, Warn, Info, Debug, Verbose
};

class Logger
{
public:
    using TimeProvider = uint32_t (*)();
    static void init(LogLevel level = LogLevel::Info);
    static void setLevel(LogLevel level);
    static LogLevel getLevel();
    static void setTimeProvider(TimeProvider time); // Ms since boot
    static void print(LogLevel level, const char* log_src, const char* log_format, ...);

private:
    static const char* levelToString(LogLevel level);
    static uint32_t getTimeMs();
    static LogLevel s_level;
    static TimeProvider s_timeProvider;
};

#define LOG_INF(log_src, ...) Logger::print(LogLevel::Info, log_src, __VA_ARGS__)
#define LOG_DEB(log_src, ...) Logger::print(LogLevel::Debug, log_src, __VA_ARGS__)
#define LOG_ERR(log_src, ...) Logger::print(LogLevel::Error, log_src, __VA_ARGS__)
#define LOG_WARN(log_src, ...) Logger::print(LogLevel::Warn, log_src, __VA_ARGS__)
#define LOG_VERB(log_src, ...) Logger::print(LogLevel::Verbose, log_src, __VA_ARGS__)
#endif // LOGGER_H