#pragma once

#include <format>
#include <iostream>
#include <memory>

template <class... T>
void LOG(std::format_string<T...> &&fmt, T &&...args)
{
    std::cout << std::format(fmt, args...) << "\n";
}

template <class... T>
void ERROR(std::format_string<T...> &&fmt, T &&...args)
{
    std::cout << "[ERROR] " << std::format(fmt, args...) << "\n";
}
