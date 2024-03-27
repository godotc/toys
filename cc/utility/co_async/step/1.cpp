


#include <coroutine>
#include <cstdio>
#include <iostream>
#include <utility>

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

    RetType value;

    // before initialze_suspend(), which do contruction before init
    std::coroutine_handle<Promise> get_return_object()
    {
        debug(), __FUNCITON_NAME__;
        return std::coroutine_handle<Promise>::from_promise(*this);
    }

    // on first initialze as a task
    auto initial_suspend()
    {
        debug(), __FUNCITON_NAME__;
        return std::suspend_always();
    }


    //..
    auto unhandled_exception() { throw; }

    // co_yield
    auto yield_value(RetType ret)
    {
        debug(), __FUNCITON_NAME__;
        value = ret;

        // wait next resume()
        return std::suspend_always();
    }

    // co_return
    void return_value(RetType ret)
    {
        debug(), __FUNCITON_NAME__;
        value = ret;
    }

    // after return value/return void
    auto final_suspend() noexcept
    {
        debug(), __FUNCITON_NAME__;
        // return or co_wait sub coroutine
        return PreviousAwaiter(m_Previous);
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
        constexpr bool await_ready() const noexcept
        {
            // suspend when not ready
            return false;
        }

        std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
        {
            if (coroutine.done()) {
                return std::noop_coroutine();
            }
            return coroutine;
        }
        constexpr void await_resume() const noexcept {}
    };
    auto operator co_await() const { return Awaiter(m_Coroutine); }
};

template <typename T>
struct WorldTask {

    using promise_type = Promise<T>;


    // curent
    std::coroutine_handle<promise_type> m_Coroutine;

    WorldTask(std::coroutine_handle<promise_type> coroutine) : m_Coroutine(coroutine) {}
    WorldTask(WorldTask &&) = delete;
    ~WorldTask() { m_Coroutine.destroy(); }

    struct WorldTaskAwaiter {
        std::coroutine_handle<promise_type> m_Coroutine;

        // blocking
        constexpr bool await_ready() const noexcept { return false; }

        std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
        {
            // this arg `coroutine` let you know which coroutine call this sub coroutine
            // (who's your dady) and we store it
            m_Coroutine.promise().m_Previous = coroutine;

            // return self's coroutine context
            return m_Coroutine;
        }
        constexpr void await_resume() const noexcept {}
    };

    auto operator co_await() const noexcept { return WorldTaskAwaiter{.m_Coroutine = m_Coroutine}; }
};


WorldTask<int> world()
{
    debug(), "\t\t[world]", "from world's 22!";
    co_yield 22; // yield will be recived by  the top level coroutine?
    debug(), "\t\t[world]", "from world's 32!";
    co_yield 32;
    debug(), "\t\t[world]", "from world's 42!";
    co_return 42;
}

Task<int> hello()
{
    debug(), "\t[hello] world_task contructing...";
    WorldTask<int> world_task = world();
    debug(), "\t[hello] world_task contruct completed! co_await it....";
    debug(), "\t[hello] world_task co_await complete", "return value:", world_task.m_Coroutine.promise().value;
    co_await world_task;
    debug(), "\t[hello] world_task co_await complete", "return value:", world_task.m_Coroutine.promise().value;
    co_await world_task;
    debug(), "\t[hello] world_task co_await complete", "return value:", world_task.m_Coroutine.promise().value;

    debug();

    printf("hello 12!\n");
    co_yield 12;
    printf("hello 13!\n");
    co_yield 13;
    printf("hello 14!\n");
    co_yield 14;
    printf("hello 20!\n");
    co_return 20;
}


int main()
{
    debug(), "before hello task contruct";
    Task t = hello();
    debug(), "after hello task contruct";
    debug();

    int i = 0;

    while (!t.m_Coroutine.done()) {
        t.m_Coroutine.resume();
        debug(), "[main] revice return value from hello:", i++, "->", t.m_Coroutine.promise().value;
    }

    return 0;
}
