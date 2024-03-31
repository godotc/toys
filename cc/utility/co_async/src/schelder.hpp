#pragma once

#include <chrono>
#include <coroutine>
#include <deque>
#include <queue>

#include "ut.hpp"

struct Scheduler {
    struct TimerEntry {
        std::chrono::system_clock::time_point expeired_time;
        std::coroutine_handle<>               handle;

        bool operator<(const TimerEntry &other) const
        {
            // NOTICE: the priority_queue is the big top head! need a reverse
            // we need the smaller timepint(earlier) at the top
            return expeired_time > other.expeired_time;
        }
    };


    std::deque<std::coroutine_handle<>> m_ReadyQueue;
    std::priority_queue<TimerEntry>     m_TimerHeap;


    Scheduler &operator=(Scheduler &&) = delete;

    void AddTask(std::coroutine_handle<> handle)
    {
        m_ReadyQueue.push_front(handle);
    }
    void AddTimer(std::chrono::system_clock::time_point expeired_time, std::coroutine_handle<> handle)
    {
        m_TimerHeap.push({expeired_time, handle});
    }

    void RunAll()
    {
        while (!m_TimerHeap.empty() || !m_ReadyQueue.empty())
        {
            // debug(), "Num timer and read: ", m_Timers.size(), ", ", m_ReadyQueue.size();
            while (!m_ReadyQueue.empty())
            {
                // ptr
                auto task = m_ReadyQueue.front();
                debug(), "pop";
                m_ReadyQueue.pop_front();
                task.resume();
            }

            auto now = std::chrono::system_clock::now();
            while (!m_TimerHeap.empty())
            {
                // we use const ref
                const auto &timer = m_TimerHeap.top();
                if (timer.expeired_time > now) {
                    break;
                }
                // timer.handle.resume();

                // so need to push then pop (or nullable access)
                m_ReadyQueue.push_front(timer.handle);
                m_TimerHeap.pop();
            };
        }
    }
};


// TODO: don't know what will happen
inline Scheduler &GetScheduler()
{
    static Scheduler scheduler;
    return scheduler;
}
