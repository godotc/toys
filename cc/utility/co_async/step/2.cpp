


#include <coroutine>
#include <exception>
#include <iostream>
#include <stacktrace>

#include "ut.hpp"



/*
 * 3 interfaces `Awaitable` required for compiler on coroutine
 * system call process:
 * if await_ready() is ture, go to await_resume(). Or call the await_suspend()
 */
struct SuspendNenver {
    constexpr bool await_ready() const noexcept
    {
        // never stop if returned by initialzied/yield_value/return_value
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

// struct RepeatAwaitable {
//  implement this for some struct, define it's awaitable tpye inplementations
//    RepeatAwaiter operator co_await() { return RepeatAwaiter(); }
//};



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
    // call a task,  and in this task call co_await() another task,
    // and the first task's caller will receive return value by the
    // co_await's task
    std::coroutine_handle<> m_Previous = nullptr;
    std::exception_ptr      m_ExceptionPtr;
    union
    {
        RetType m_Result;
    };
    Promise() noexcept  = default;
    Promise(Promise &&) = delete;
    ~Promise() noexcept = default;

    // before initialze_suspend(), which do contruction before init
    std::coroutine_handle<Promise> get_return_object() { return std::coroutine_handle<Promise>::from_promise(*this); }

    // on first initialze as a task -> block
    auto initial_suspend() { return std::suspend_always(); }

    // after return value/return void
    auto final_suspend() noexcept { return PreviousAwaiter(m_Previous); /* handle sub coroutine*/ }

    //..
    auto unhandled_exception()
    {
        debug(), "unhandled_exception";
        // 1. throw; // equivalent with -> std::rethrow_exception(std::current_exception());
        // 2. // throw; if still do not handle, will be ignored

        // 3. store it
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
            // auto st = std::stacktrace::current();
            // std::cerr << std::to_string(st);
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
};



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
    debug(), "hello";
    auto i = co_await world();
    co_await nobody();
    co_return i + 1;
    debug(), "hello";
}



int main()
{
    Task t = hello();

    int i = 0;

    while (!t.m_Coroutine.done()) {
        t.m_Coroutine.resume();
        debug(), "[main] revice return value from hello:", i++, "->", t.m_Coroutine.promise().result();
    }

    return 0;
}
