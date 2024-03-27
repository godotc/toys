/**
 *  Author: @godot42
 *  Create Time: 2024-03-28 04:35:12
 *  Modified by: @godot42
 *  Modified time: 2024-03-28 04:50:57
 *  Description:
 */




#include <chrono>
#include <coroutine>
#include <deque>
#include <exception>
#include <queue>
#include <thread>

#include "ut.hpp"

using namespace std::chrono_literals;

struct SuspendNever {
    constexpr bool await_ready() const noexcept
    {
        // never stop if returned by initialized/yield_value/return_value
        return true;
    }

    std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
    {
        if (coroutine.done()) {
            return std::noop_coroutine();
        }
        // request next call to self
        return coroutine;
    }
    constexpr void await_resume() const noexcept {}
};



struct PreviousAwaiter {
    std::coroutine_handle<> m_Previous;

    constexpr bool await_ready() const noexcept
    {
        // block it
        return false;
    }

    std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
    {
        // has parent coroutine
        if (m_Previous) {
            return m_Previous;
        }
        //  stop
        return std::noop_coroutine();
    }
    constexpr void await_resume() const noexcept {};
};



template <typename RetType>
struct Promise {
    std::coroutine_handle<> m_Previous = nullptr;
    std::exception_ptr      m_ExceptionPtr;
    union
    {
        RetType m_Result;
    };
    Promise() noexcept  = default;
    Promise(Promise &&) = delete;
    ~Promise() noexcept = default;

    // before initialize_suspend(), which do construction before init
    std::coroutine_handle<Promise> get_return_object() { return std::coroutine_handle<Promise>::from_promise(*this); }

    // on first initialize as a task -> block
    auto initial_suspend() { return std::suspend_always(); }

    // after return value/return void
    auto final_suspend() noexcept { return PreviousAwaiter(m_Previous); /* handle sub coroutine*/ }

    //..
    auto unhandled_exception()
    {
        debug(), "unhandled_exception";
        m_ExceptionPtr = std::current_exception();
    }

    // co_yield
    auto yield_value(RetType ret)
    {
        new (&m_Result) RetType(std::move(ret));
        return std::suspend_always(); // block
    }

    // co_return
    void return_value(RetType ret)
    {
        new (&m_Result) RetType(std::move(ret));
    }



    RetType result()
    {
        if (m_ExceptionPtr) [[unlikely]] {
            std::rethrow_exception(m_ExceptionPtr);
        }

        RetType v = std::move(m_Result);
        m_Result.~RetType();
        return v;
    }
};

template <>
struct Promise<void> {
    std::coroutine_handle<> m_Previous = nullptr;
    std::exception_ptr      m_ExceptionPtr{};

    std::coroutine_handle<Promise> get_return_object() { return std::coroutine_handle<Promise>::from_promise(*this); }

    auto initial_suspend() { return std::suspend_always(); }

    auto unhandled_exception() { m_ExceptionPtr = std::current_exception(); }

    auto yield_value() { m_ExceptionPtr = nullptr; }
    void return_void() { m_ExceptionPtr = nullptr; }

    // after return value/return void
    auto final_suspend() noexcept { return PreviousAwaiter(m_Previous); /* handle sub coroutine*/ }

    void result()
    {
        if (m_ExceptionPtr) [[unlikely]] {
            std::rethrow_exception(m_ExceptionPtr);
        }
    }
};


template <typename T>
struct Task {
    using promise_type = Promise<T>;

    // curent
    std::coroutine_handle<promise_type> m_Coroutine;

    Task(std::coroutine_handle<promise_type> coroutine) : m_Coroutine(coroutine) {}
    Task(Task &&) = delete;
    ~Task() { m_Coroutine.destroy(); }

    struct Awaiter {
        std::coroutine_handle<promise_type> m_Coroutine;

        constexpr bool await_ready() const noexcept
        {
            // suspend when not ready
            return false;
        }

        std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
        {
            m_Coroutine.promise().m_Previous = coroutine;
            return m_Coroutine;
        }
        constexpr T await_resume() const noexcept
        {
            return m_Coroutine.promise().result();
        }
    };
    auto operator co_await() const { return Awaiter(m_Coroutine); }
    operator std::coroutine_handle<>() const { return m_Coroutine; }
};

struct Scheduler {

    struct TimerEntry {
        std::chrono::system_clock::time_point expired_time;
        std::coroutine_handle<>               handle;

        bool operator<(const TimerEntry &other) const
        {
            // NOTICE: the priority_queue is the big top head! need a reverse
            // we need the smaller timepoint (earlier) at the top
            return expired_time > other.expired_time;
        }
    };


    std::deque<std::coroutine_handle<>> m_ReadyQueue;
    std::priority_queue<TimerEntry>     m_Timers;


    Scheduler &operator=(Scheduler &&) = delete;

    void AddTask(std::coroutine_handle<> handle)
    {
        m_ReadyQueue.push_front(handle);
    }
    void AddTimer(std::chrono::system_clock::time_point expired_time, std::coroutine_handle<> handle)
    {
        m_Timers.push({expired_time, handle});
    }

    void RunAll()
    {
        while (!m_Timers.empty() || !m_ReadyQueue.empty())
        {
            while (!m_ReadyQueue.empty())
            {
                auto task = m_ReadyQueue.front();
                m_ReadyQueue.pop_front();
                task.resume();
            }

            auto now = std::chrono::system_clock::now();
            while (!m_Timers.empty())
            {
                const auto &timer = m_Timers.top();
                if (timer.expired_time > now) {
                    break;
                }

                // timer.handle.resume();
                m_Timers.pop();
                m_ReadyQueue.push_front(timer.handle);
            };
        }
    }
};


Scheduler &GetScheduler()
{
    static Scheduler scheduler;
    return scheduler;
}



struct SleepAwaiter {
    using promise_type = Promise<void>;

    std::chrono::system_clock::time_point m_ExpiredTime;

    std::coroutine_handle<promise_type> m_Coroutine;

    bool await_ready() const noexcept
    {
        return std::chrono::system_clock::now() >= m_ExpiredTime;
    }

    std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
    {
        // synchronous way
        // std::this_thread::sleep_until(m_ExpiredTime);
        // return coroutine;

        GetScheduler().AddTimer(m_ExpiredTime, coroutine);
        return std::noop_coroutine();
    }
    constexpr void await_resume() const noexcept {}
};


Task<void> sleep_until(std::chrono::system_clock::time_point tp)
{
    co_await SleepAwaiter(tp);
    co_return;
}

Task<void> sleep_for(std::chrono::system_clock::duration dur)
{
    co_await SleepAwaiter(std::chrono::system_clock::now() + dur);
    co_return;
}


Task<void> nobody()
{
    debug(), "nobody";
    co_return;
}
Task<int> world()
{
    debug(), "world";
    // throw std::runtime_error("world failed");
    // throw "world failed"; this cannot print what() by exception handler
    co_return 42;
}

Task<int> hello()
{
    auto i = co_await world();
    co_await nobody();

    co_await sleep_for(1s);
    debug(), "hello";
    co_await sleep_for(3s);
    debug(), "hello";
    co_return i + 1;
}
Task<int> hello2()
{
    auto i = co_await world();

    co_await sleep_for(1ms);
    debug(), "hello2";
    co_await sleep_for(2s);
    debug(), "hello2";
    co_return i + 1;
}



int main()
{
    try {
        Task t  = hello();
        auto t2 = hello2();
        GetScheduler().AddTask(t);
        GetScheduler().AddTask(t2);

        GetScheduler().RunAll();

        debug(), t.m_Coroutine.promise().result();
        debug(), t2.m_Coroutine.promise().result();
    }
    catch (std::exception &exception) {
        debug(), exception.what();
    }


    return 0;
}
