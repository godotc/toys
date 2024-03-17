#include "logger.h"

#include <bits/types/time_t.h>
#include <pthread.h>

#include <chrono>
#include <condition_variable>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

using namespace gstl::utility;

char *Logger::pp_level[LEVEL_COUNT]{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
};

Logger *Logger::p_instance = nullptr;


Logger::Logger()
{
    std::thread(worker());
}

Logger::~Logger()
{
    m_isRunning = false;

    for (auto &[name, appender] : m_appenders)
    {
        appender->m_stream.close();
        delete appender;
    }
}

void Logger::worker()
{
    while (running())
    {
        Msg message;
        m_blockQueue.pop(message);

        const char *msg   = message.p_msg;
        Level       level = message.m_level;


        if (m_listenLevel >= level)
            std::cout << msg << std::endl;

        if (msg == nullptr)
            continue;


        for (auto &[name, appender] : m_appenders)
        {
            if (appender->level >= level)
            {
                appender->m_stream << msg;
                appender->m_stream.flush();
            }
            else
                continue;

            if (appender->len >= appender->m_max)
            {
                // TODO(fix): create a new logfile when beyond the max size;
            }
        }

        delete msg;
    }
}


void Logger::log(Level level, const char *file, int line, const char *format, ...)
{
    if (level < m_appenderLevel)
        return;
    char    timestamp[32]; // 时间戳
    va_list arg_ptr;       // format 内容指针
    char   *buffer;        // log 总共的 buffer

    // time str
    time_t     now  = time(nullptr);
    struct tm *p_tm = localtime(&now);
    std::memset(timestamp, 0, sizeof(timestamp));
    ::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", p_tm);
    /*
    auto        _now     = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(_now);
    std::string timestr  = std::ctime(&end_time);*/


    // status fmt size
    const char *fmt         = "%s [%s] %s:%d ";
    int         status_size = snprintf(NULL, 0, fmt, timestamp, pp_level[level], file, line);

    //  content msg size
    ::va_start(arg_ptr, format);
    int content_size = vsnprintf(NULL, 0, format, arg_ptr); // vnsprintf
    ::va_end(arg_ptr);

    // format
    if (status_size + content_size > 0)
    {
        int total_size = status_size + content_size + 1;
        buffer         = new char[total_size];

        snprintf(buffer, status_size + 1, fmt, timestamp, pp_level[level], file, line);

        ::va_start(arg_ptr, format);
        vsnprintf(buffer + status_size, content_size + 1, format, arg_ptr);
        ::va_end(arg_ptr);
        buffer[total_size] = '\0';
    }

    Msg msg{buffer, level};
    m_blockQueue.push(msg);
}

Logger *Logger::getInstance()
{
    if (p_instance == nullptr) {
        p_instance = new Logger;
    }
    return p_instance;
}

void Logger::addAppender(const std::string &filename, Level level)
{
    Appender *appender = new Appender(filename, level);

    m_appenders.insert({filename, appender});
}


void Logger::closeAppender(const std::string &logname)
{
    auto it = m_appenders.find(logname);
    if (it != m_appenders.end()) {
        it->second->m_stream.close();
        delete it->second;
    }
    m_appenders.erase(logname);
}

// 0-4 debug-fatal
void Logger::seListenLevel(Level level)
{
    m_listenLevel = level;
}

void Logger::setMaxFileSize(int size)
{
    m_fileMaxSize = size;
}

constexpr bool Logger::running() { return m_isRunning; }

Logger::Appender::Appender(const std::string &name, Level level) : m_max(10000000), len(0), level(level)
{
    std::string filename = std::string(LOG_PREFIX) + pp_level[level] + '-' + name;
    m_stream.open(filename, std::ios::app);
    if (m_stream.fail()) {
        throw std::runtime_error("error while create Appender: open logfile failed! ");
    }
}

Logger::Appender::~Appender()
{
    m_stream.close();
}
