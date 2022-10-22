
#include <cstdio>
#include <utility>

#include "logger.h"

using namespace gstl::utility;

int main()
{
    Logger* logger = Logger::getInstance();
    logger->addAppender("teset.log", gstl::utility::Logger::WARN);

    logger->log(Logger::DEBUG, __FILE__, __LINE__, "hello %s", "jerry");

    g_debug("hello %s", "marco debug");
    g_info("hello %s", "marco info");
    g_warn("hello %s", "marco warn");
    g_error("hello %s", "marco error");
    g_fatal("hello %s", "marco fatal");


    while (getchar() != 'q') {
    }

	return 0;
}
