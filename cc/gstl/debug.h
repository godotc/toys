#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>


enum class DEBUG_LEVEL : uint8_t
{
    DEBUG = 0,
    TRACE,
    INFO,
    WARN,
    ERROR,
    FATAL
};

constexpr DEBUG_LEVEL LOG_LEVEL = DEBUG_LEVEL::DEBUG;


#define LOG(LEVEL, ...)                                                 \
    do {                                                                \
        if (LEVEL >= LOG_LEVEL) {                                       \
            const char *format = "[%s] %s\n";                           \
            auto        len    = sprintf(format, ##LEVEL, __VA_ARGS__); \
            char        msg[len];                                       \
            printf(msg, format, ##LEVEL, __VA_ARGS__);                  \
            std::cout << msg << std::endl;                              \
        }                                                               \
    } while (0)

#define DBG(...) LOG(DEBUG_LEVEL::WARN, __VA_ARGS__)


static void test()
{
}
