#pragma once
#ifndef GLOG_UTIL_LOGGER
#define GLOG_UTIL_LOGGER

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "block_queue.hpp"

namespace gstl {
namespace utility {



#define g_debug(format, ...) \
    Logger::getInstance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define g_info(format, ...) \
    Logger::getInstance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define g_warn(format, ...) \
    Logger::getInstance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define g_error(format, ...) \
    Logger::getInstance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define g_fatal(format, ...) \
    Logger::getInstance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)


#define LOG_PREFIX "log-"

class Logger
{
   public:
    enum Level
    {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT
    };
    static Logger* getInstance();
    void           worker();

    void log(Level level, const char* file, int line, const char* format, ...);

    void addAppender(const std::string& filename, Level level);
    void closeAppender(const std::string& logname);

    void seListenLevel(Level level);
    void setMaxFileSize(int size);

    constexpr bool running();


   private:
    Logger();
    ~Logger();

   private:
    static Logger* p_instance;             // 单例
    static char*   pp_level[LEVEL_COUNT];  // level string

    bool m_isRunning{true};  // 是否在运行

    struct Appender
    {
        std::ofstream m_stream;
        int           m_max;
        int           len;
        Level         level;

        Appender(const std::string& name, Level level);
        ~Appender();
    };

    std::unordered_map<std::string, Appender*> m_appenders;


    Level m_listenLevel;  // 标准输出level;
    Level m_appenderLevel;

    int m_fileMaxSize{10000000};


    struct Msg
    {
        char* p_msg{nullptr};
        Level m_level{DEBUG};
    };

    BlockQueue<Msg> m_blockQueue;
};



}  // namespace utility
}  // namespace gstl
#endif
