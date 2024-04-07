#pragma once

#include <chrono>
#include <coroutine>
#include <optional>
#include <sys/epoll.h>
#include <unistd.h>

#include "base.hpp"
#include "rbtree.hpp"


namespace co_async {


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


struct TimerLoop {
    // did not owning the promise?
    RBTree<SleepUntilPromise> m_TimeerRBTree{};

    TimerLoop &operator=(TimerLoop &&) = delete;



    void AddTimer(SleepUntilPromise &promise) { m_TimeerRBTree.insert(promise); }
    bool HasEvent() const noexcept { return !m_TimeerRBTree.empty(); }

    std::optional<std::chrono::system_clock::duration> Run()
    {
        while (!m_TimeerRBTree.empty())
        {
            auto  now     = std::chrono::system_clock::now();
            auto &promise = m_TimeerRBTree.front();
            if (promise.m_ExpireTime < now) {
                m_TimeerRBTree.erase(promise);
                std::coroutine_handle<SleepUntilPromise>::from_promise(promise)
                    .resume();
            }
            else {
                return promise.m_ExpireTime - now;
            }
        }
        return std::nullopt;
    }
};


// TODO: don't know what will happen
inline TimerLoop &GetTimerLoop()
{
    static TimerLoop scheduler;
    return scheduler;
}



struct SleepAwaiter {
    using ClockType = std::chrono::system_clock;


    TimerLoop            &schelder;
    ClockType::time_point m_ExpireTime;

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<SleepUntilPromise> coroutine) const noexcept
    {
        auto &promise        = coroutine.promise();
        promise.m_ExpireTime = m_ExpireTime;
        schelder.AddTimer(promise);
    }
    constexpr void await_resume() const noexcept {}
};


template <typename Clock, typename Dur>
inline Task<void, SleepUntilPromise>
sleep_until(TimerLoop &loop, std::chrono::time_point<Clock, Dur> tp)
{
    co_await SleepAwaiter(
        loop,
        std::chrono::time_point_cast<SleepAwaiter::ClockType::duration>(tp));
}

inline Task<void, SleepUntilPromise> sleep_for(TimerLoop &loop, std::chrono::system_clock::duration dur)
{
    auto d = std::chrono::duration_cast<SleepAwaiter::ClockType::duration>(dur);
    if (d.count() > 0) {
        co_await SleepAwaiter(loop, SleepAwaiter::ClockType::now() + d);
    }
}
} // namespace co_async
