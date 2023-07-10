#pragma once

#include "fmt/core.h"
#include "log.h"
#include "pyvm/pyvm.h"
#include <memory>
#include <string.h>


PYVM_TOP_LEVEL_NAMESPACE_BEGIN

#define USE_ARRAYLIST_INSTEAD_OF_VECTOR
#ifdef USE_ARRAYLIST_INSTEAD_OF_VECTOR

template <class T>
class PyArrayList
{
  public:
    PyArrayList(int n = 8)
    {
        m_Capacity = n;
        m_Size     = 0;
        m_Array    = new T[n];
    }
    virtual ~PyArrayList()
    {
        delete[] m_Array;
    }

    T    get(int index) { return m_Array[index]; }
    void set(int index, T t) { m_Array[index] = t; }
    int  size() { return m_Size; }
    int  capacity() { return m_Capacity; }

    void push_back(T t)
    {
        if (m_Size >= m_Capacity) {
            expand();
        }

        m_Array[m_Size++] = t;
    }

    void insert(int index, T t)
    {
        // NOTICE: push what?
        push_back(T());

        for (int i = m_Size; i > index; --i) {
            m_Array[i] = m_Array[i - 1];
        }
        m_Array[index] = t;
    }


    void pop_back()
    {
        return m_Array[--m_Size];
    }

  private:
    void expand()
    {
        int new_size  = m_Capacity << 1;
        T  *new_array = new T[new_size];
        memcpy(new_array, m_Array, m_Capacity);

        delete[] m_Array;
        m_Array    = new_array;
        m_Capacity = new_size;

        LOG_DEBUG("Expand a array 0x{:x} to {}, size is {}", reinterpret_cast<std::uintptr_t>(this), m_Capacity, m_Size);
    }

  private:
    T  *m_Array;
    int m_Size;
    int m_Capacity;
};

#endif

PYVM_TOP_LEVEL_NAMESPACE_END
