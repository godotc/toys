#pragma once

#include <chrono>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <system_error>
#ifdef WIN32
    #define __FUCNTION_NAME__ __FUNCSIG__
#else
    #define __FUNCITON_NAME__ __PRETTY_FUNCTION__
#endif


template <class T>
inline std::ostream &operator<<(std::ostream &out, std::optional<T> v)
{
    if (v.has_value()) {
        out << v.value();
    }
    else {
        out << "[None]";
    }
    return out;
}



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


namespace co_async {

} // namespace co_async
