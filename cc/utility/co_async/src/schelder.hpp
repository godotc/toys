#pragma once

#include <chrono>
#include <coroutine>
#include <thread>

#include "base.hpp"
#include "rbtree.hpp"



struct SleepUntilPromise
    : public RBTree<SleepUntilPromise>::RBNode,
      public Promise<void> {


    std::chrono::system_clock::time_point m_ExpireTime;

    Promise &operator=(Promise &&) = delete;

    auto get_return_object()
    {
        return std::coroutine_handle<SleepUntilPromise>::from_promise(*this);
    }


    friend bool operator<(SleepUntilPromise const &lhs, SleepUntilPromise const &rhs) noexcept
    {
        return lhs.m_ExpireTime < rhs.m_ExpireTime;
    }
};


struct Scheduler {

    // did not owning the promise?
    RBTree<SleepUntilPromise> m_TimeerRBTree{};

    Scheduler &operator=(Scheduler &&) = delete;

    void AddTimer(SleepUntilPromise &promise)
    {
        m_TimeerRBTree.insert(promise);
    }

    void RunAll(std::coroutine_handle<> coroutine)
    {
        while (!coroutine.done()) {
            coroutine.resume();
            while (!m_TimeerRBTree.empty()) {
                if (!m_TimeerRBTree.empty()) {
                    auto  now     = std::chrono::system_clock::now();
                    auto &promise = m_TimeerRBTree.front();
                    if (promise.m_ExpireTime < now) {
                        m_TimeerRBTree.erase(promise);
                        std::coroutine_handle<SleepUntilPromise>::from_promise(promise).resume();
                    }
                    else {
                        std::this_thread::sleep_until(promise.m_ExpireTime);
                    }
                }
            }
        }
    }
};


// TODO: don't know what will happen
inline Scheduler &GetScheduler()
{
    static Scheduler scheduler;
    return scheduler;
}



struct SleepAwaiter {
    Scheduler                            &schelder;
    std::chrono::system_clock::time_point m_ExpireTime;

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<SleepUntilPromise> coroutine) const noexcept
    {
        auto &promise        = coroutine.promise();
        promise.m_ExpireTime = m_ExpireTime;
        schelder.AddTimer(promise);
    }
    constexpr void await_resume() const noexcept {}
};


inline Task<void, SleepUntilPromise> sleep_until(std::chrono::system_clock::time_point tp)
{
    auto &schelder = GetScheduler();
    co_await SleepAwaiter(schelder, tp);
}

inline Task<void, SleepUntilPromise> sleep_for(std::chrono::system_clock::duration dur)
{
    auto &schelder = GetScheduler();
    co_await SleepAwaiter(schelder, std::chrono::system_clock::now() + dur);
}
