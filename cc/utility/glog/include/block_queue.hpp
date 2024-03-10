#pragma once
#include <sys/types.h>

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <ratio>
#include <stdexcept>
#include <string>
#include <type_traits>
#ifndef GSTL_BLOCK_QUEUE_HPP
#define GSTL_BLOCK_QUEUE_HPP

#include <mutex>

#include "locker.hpp"

namespace gstl {

template <typename T>
class BlockQueue
{
   public:
    BlockQueue(int max_size = 1000)
        : m_maxSize(max_size),
          m_array(new T[max_size]),
          m_size(0),
          m_front(-1)
    {
        if (max_size <= 0)
            throw std::logic_error("contruct blck queue error");
    }

    ~BlockQueue()
    {
        gstl::LockGuard lock(&m_mutex);
        if (m_array != nullptr) {
            delete[] m_array;
            m_array = nullptr;
        }
    }

   public:
    void clear()
    {
        gstl::LockGuard lock(&m_mutex);
        m_size  = 0;
        m_front = m_back = -1;
    }

    bool full()
    {
        gstl::LockGuard lock(&m_mutex);
        return m_size >= m_maxSize ? true : false;
    }
    bool empty()
    {
        gstl::LockGuard lock(&m_mutex);
        return m_size == 0 ? true : false;
    }
    int size()
    {
        gstl::LockGuard lock(&m_mutex);
        return m_size;
    }
    int max_size()
    {
        gstl::LockGuard lock(&m_mutex);
        return m_maxSize;
    }


    bool push(const T& item)
    {
        gstl::LockGuard lock(&m_mutex);

        if (m_size >= m_maxSize) {
            m_cond.notify_all();
            return false;
        }

        m_back          = (m_back + 1) % m_maxSize;
        m_array[m_back] = item;
        ++m_size;

        m_cond.notify_all();
        return true;
    }
    bool pop(T& item)
    {
        std::unique_lock<std::mutex> u_lock(m_mutex);

        // if no item wait
        while (m_size <= 0) {
            m_cond.wait(u_lock);
        }

        m_front = (m_front + 1) % m_maxSize;
        item    = m_array[m_front];
        m_size--;

        return true;
    }

    bool pop(T& item, int ms_timeout)
    {
        std::unique_lock<std::mutex> u_lock(m_mutex);

        if (m_size <= 0) {
            // m_cond.wait_for(u_lock, std::chrono::duration<u_int64_t, std::milli>(ms_timeout));
            if (m_cond.wait_for(u_lock, std::chrono::milliseconds(ms_timeout)) == std::cv_status::timeout)
                return false;
        }

        if (m_size <= 0) return false;

        m_front = (m_front + 1) % m_maxSize;  // start from -1, so first ++ ,then get
        item    = m_array[m_front];
        --m_size;

        return true;
    }

   private:
    bool front(T& val)
    {
        gstl::LockGuard lock(&m_mutex);

        if (0 == m_size) return false;
        val = m_array[m_front];
        return true;
    }
    bool back(T& val)
    {
        gstl::LockGuard lock(&m_mutex);

        if (0 == m_size) return false;
        val = m_array[m_back];
        return true;
    }

   private:
    std::mutex              m_mutex;
    std::condition_variable m_cond;

    T*  m_array;
    int m_size;
    int m_maxSize;
    int m_front;
    int m_back;
};


}  // namespace gstl
#endif
