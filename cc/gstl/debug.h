#pragma once

#include <cstdarg>
#include <iostream>

#ifdef DEBUG
#define LOG(...)                                \
    do {                                        \
        std::cout << __VAR_ARGS__ << std::endl; \
    } while (0)
#endif
