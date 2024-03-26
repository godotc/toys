


#include <coroutine>
#include <cstdio>



struct LoopAwaiter {
    constexpr bool          await_ready() const noexcept { return false; }
    std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
    {
        if (coroutine.done()) {
            return std::noop_coroutine();
        }
        return coroutine;
    }
    constexpr void await_resume() const noexcept {}
};

struct LoopAwaitable {

    LoopAwaiter operator co_await() { return LoopAwaiter(); }
};


template <typename RetType>
struct Promise {
    RetType value;


    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    auto unhandled_exception() { throw; }
    void return_value(RetType ret) { value = ret; }

    auto yield_value(RetType ret)
    {
        value = ret;
        return std::suspend_always();
    }

    std::coroutine_handle<Promise> get_return_object()
    {
        return std::coroutine_handle<Promise>::from_promise(*this);
    }
};
template <>
struct Promise<void> {

    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    auto unhandled_exception() { throw; }
    void return_void() {}

    std::coroutine_handle<Promise> get_return_object()
    {
        return std::coroutine_handle<Promise>::from_promise(*this);
    }
};



template <typename T>
struct Task {
    using promise_type = Promise<T>;

    std::coroutine_handle<promise_type> m_Coroutine;

    Task(std::coroutine_handle<promise_type> coroutine)
        : m_Coroutine(coroutine) {}
};



Task<int> hello()
{
    printf("hello world!\n");
    co_yield 12;
    printf("hello world!\n");
    co_yield 13;
    printf("hello world!\n");
    co_yield 14;
    printf("hello world!\n");
    co_return 42;
}

int main()
{
    Task t = hello();
    printf("???\n");

    int i = 0;

    while (!t.m_Coroutine.done()) {
        t.m_Coroutine.resume();
        printf("%d -- : %d\n", i++, t.m_Coroutine.promise().value);
    }

    return 0;
}
