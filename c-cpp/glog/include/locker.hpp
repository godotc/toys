
#pragma once
#include <bits/types/struct_timespec.h>

#include <system_error>
#ifndef GSTL_LOCKER_HPP
#define GSTL_LOCKER_HPP

#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

#include <condition_variable>
#include <exception>
#include <mutex>
#include <stdexcept>

namespace gstl {


class Semaphore
{
   public:
    bool wait() { return sem_wait(&m_sem) == 0; }
    bool post() { return sem_post(&m_sem) == 0; }

   public:
    Semaphore()
    {
        if (sem_init(&m_sem, 0, 0) != 0)
            throw std::runtime_error("error on init semaphore");
    }
    Semaphore(int num)
    {
        if (sem_init(&m_sem, 0, num) != 0)
            throw std::runtime_error("error on init mutli(num) semphore");
    }

    ~Semaphore() { sem_destroy(&m_sem); }

   private:
    sem_t m_sem;
};

class LockGuard
{
   public:
    LockGuard(std::mutex* mtx)
    {
        m_mtx = mtx;
        m_mtx->lock();
        has_lock = true;
    }
    ~LockGuard()
    {
        m_mtx->unlock();
        has_lock = false;
    }

    bool lock()
    {
        if (!has_lock) {
            m_mtx->lock();
            return true;
        }
        else
            return false;
    }
    bool unlock()
    {
        if (has_lock) {
            m_mtx->unlock();
            return true;
        }
        else
            return false;
    }

    bool hasLock() { return has_lock; }

   private:
    bool        has_lock{false};
    std::mutex* m_mtx;
};

class Cond
{
   public:
    Cond()
    {
        if (pthread_cond_init(&m_cond, nullptr) != 0)
            throw std::exception();
    }
    ~Cond() { pthread_cond_destroy(&m_cond); }

    bool wait(pthread_mutex_t* m_mutex)
    {
        int ret = 0;
        ret     = pthread_cond_wait(&m_cond, m_mutex);
        return ret == 0;
    }

    bool timewait(pthread_mutex_t* m_mutex, struct timespec t)
    {
        int ret = 0;
        ret     = pthread_cond_timedwait(&m_cond, m_mutex, &t);
        return ret == 0;
    }

    // wake up one block thread by this
    bool signal() { return pthread_cond_signal(&m_cond) == 0; }

    // wake up all
    bool boardcast() { return pthread_cond_broadcast(&m_cond) == 0; }


   private:
    pthread_cond_t m_cond;
};


}  // namespace gstl
#endif
