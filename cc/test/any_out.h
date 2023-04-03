

#include <iostream>


template <class T>
inline void out(T args...)
{
    std::cout << args << std::endl;
}
