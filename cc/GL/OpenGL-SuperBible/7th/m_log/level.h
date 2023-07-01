#pragma once
#include <unordered_map>

#define __top_level_namespace __logcpp


#if _WIN32 || !_HAS_CXX17
#include <fmt/std.h>
namespace __top_level_namespace {
using string_view = fmt::string_view;
}
#else
#include <string_view>
namespace __top_level_namespace {
using string_view = std::string_view;
}
#endif


namespace __top_level_namespace {


enum LogLevel
{
    DEBUG = 0,
    TRACE,
    LOG,
    WARN,
    ERROR,
    FATAL,
    LevelCount,
};

inline static LogLevel LOG_LEVEL = LogLevel::LOG;
inline auto            SetLogLevel(LogLevel level) { LOG_LEVEL = level; }

inline char const *LogLevelStrings[] = {
    "DEBUG",
    "TRACE",
    "LOG",
    "WARN",
    "ERROR",
    "FATAL",
};

inline static std::unordered_map<std::string, std::string>
    TerminalColor =
        {
            {  "reset",  "\033[0m"},
            {  "white", "\033[37m"},
            {  "green", "\033[32m"},
            {"magenta", "\033[35m"},
            {   "cyan", "\033[36m"},
            {   "blue", "\033[34m"},
            {  "yello", "\033[33m"},
            {    "red", "\033[31m"},
};


inline auto get_terminal_color(LogLevel level)
{
    string_view color = TerminalColor["white"];

    switch (level) {
    case LOG:
        break;
    case TRACE:
        color = TerminalColor["green"];
        break;
    case DEBUG:
        color = TerminalColor["cyan"];
        break;
    case WARN:
        color = TerminalColor["yello"];
        break;
    case ERROR:
    case FATAL:
        color = TerminalColor["red"];
        break;
    case LevelCount:
        break;
    }
    return color;
}
} // namespace __top_level_namespace
