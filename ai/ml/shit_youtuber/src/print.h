#pragma once

#include <cstdint>
#include <iostream>


template <class... T>
void print(const char *fmt, T... args)
{
    printf(fmt, args...);
    printf("\n");
}

template <class T>
void print(T value)
{
    std::cout << value << '\n';
}
