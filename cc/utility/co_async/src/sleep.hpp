#pragma once

#include "schelder.hpp"

namespace co_async {

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
} // namespace co_async
