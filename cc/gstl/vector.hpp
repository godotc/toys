#define DEBUG
#include "debug.h"

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <sys/types.h>

namespace gstl {

template <typename __Type>
class vector
{
  public:
    vector();
    ~vector();

    void push_back(__Type value);
    void push_back(std::initializer_list<__Type> &&values);
    // void push(__Type &value);
    // void push(__Type &&value);

    __Type pop_back();


  private:

    void extend();

  private:
    __Type *begin;
    __Type *cur;
    __Type *end;

    size_t capacity;
    size_t size;
};

template <typename __Type>
vector<__Type>::vector()
{
    size     = 0;
    capacity = 10;
    begin    = new __Type[capacity];
    cur      = begin;
    end      = begin + capacity;
}
template <typename __Type>
vector<__Type>::~vector()
{
    if (begin) {
        delete[] begin;
    }
    begin = cur = end = nullptr;
    size = capacity = 0;
}

template <typename __Type>
void vector<__Type>::extend()
{
    LOG("hello world");
    auto new_size = capacity * 2;
    auto offset   = cur - begin;

    begin = new __Type[new_size];
    cur   = begin + offset;
    end   = cur + new_size;
}


template <typename __Type>
void vector<__Type>::push_back(__Type value)
{
    if (cur == end) {
        this->extend();
    }
    *cur = value;
    ++cur;
    ++size;
}
template <typename __Type>
void vector<__Type>::push_back(std::initializer_list<__Type> &&values)
{
    for (auto &x : values)
    {
        this->push_back(x);
    }
}

template <typename __Type>
__Type vector<__Type>::pop_back()
{
    if (size == 0) {
        std::cerr << "vector is empty!" << std::endl;
        return __Type();
    }

    --size;
    --cur;
    auto value = *cur;
    return value;
}

} // namespace gstl
