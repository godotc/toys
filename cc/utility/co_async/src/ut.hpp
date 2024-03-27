#pragma once

#include <chrono>
#include <cstddef>
#include <iostream>
#ifdef WIN32
    #define __FUCNTION_NAME__ __FUNCSIG__
#else
    #define __FUNCITON_NAME__ __PRETTY_FUNCTION__
#endif


struct debug {
    debug &operator,(auto arg)
    {
        std::cout << arg;
        return *this;
    }
    ~debug() { std::cout << '\n'; }
};
// static auto d = debug{};



inline std::chrono::seconds operator""_s(unsigned long long value)
{
    return std::chrono::seconds(value);
}
