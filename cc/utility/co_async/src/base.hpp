#pragma once

#include <coroutine>
#include <exception>
#include <functional>


template <class T = void>
struct NonVoidHelper {
    using type = T;
};
template <>
struct NonVoidHelper<void> {
    using type = NonVoidHelper;

    explicit NonVoidHelper() = default;

    template <class T>
    constexpr friend T operator,(T &&t, NonVoidHelper)
    {
        return std::forward<T>(t);
    }
};

template <class T>
struct Uninitialized {
    union
    {
        T value;
    };
    Uninitialized() noexcept {}
    ~Uninitialized() noexcept {}
    Uninitialized(Uninitialized &&) = delete;

    T move_value()
    {
        T ret(std::move(value));
        value.~T();
        return ret;
    }

    template <class... Ts>
    void put_values(Ts &&...args)
    {
        new (std::addressof(value)) T(std::forward<Ts>(args)...);
    }
};

template <>
struct Uninitialized<void> {
    auto move_value() { return NonVoidHelper<>{}; }
    void put_values(NonVoidHelper<>) {}
};

template <typename T>
struct Uninitialized<T const> : Uninitialized<T> {};

template <typename T>
struct Uninitialized<T &> : Uninitialized<std::reference_wrapper<T>> {};


template <typename T>
struct Uninitialized<T &&> : Uninitialized<T> {};

//-------------------------------------------------------


// clang-format off
template <class T>
concept Awaiter = requires(T a, std::coroutine_handle<> h) {
    { a.await_ready() };
    { a.await_suspend(h) };
    { a.await_resume() };
};

template <class T>
concept Awaitable = Awaiter<T> || requires(T a){
	{a.operator co_await()} -> Awaiter;
};

template<typename> struct AwaitableTraits;

template<Awaiter A> struct AwaitableTraits<A>{
	using RetType =decltype(std::declval<A>().await_resume());
	using NonVoidRetType = NonVoidHelper<RetType>::type;
};


template<class A>
requires(!Awaiter<A> && Awaitable<A>)
struct AwaitableTraits<A>:
	AwaitableTraits<decltype(std::declval<A>().operator co_await())> {};
// clang-format on


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
    Uninitialized<RetType>  m_Result;

    // Promise() noexcept  = default;
    // Promise(Promise &&) = delete;
    //~Promise() noexcept = default;
    Promise &operator=(Promise &&) = delete;

    // before initialze_suspend(), which do contruction before init
    std::coroutine_handle<Promise> get_return_object() { return std::coroutine_handle<Promise>::from_promise(*this); }
    // on first initialze as a task -> block
    auto initial_suspend() { return std::suspend_always(); }
    // after return value/return void
    auto final_suspend() noexcept { return PreviousAwaiter(m_Previous); /* handle sub coroutine*/ }
    auto unhandled_exception() { m_ExceptionPtr = std::current_exception(); }

    // co_yield
    /* disable yield in task, can only co_return
    auto yield_value(RetType ret)
    {
        new (&m_Result) RetType(std::move(ret));
        return std::suspend_always(); // block
    }*/

    // co_return
    void return_value(RetType &&ret) { m_Result.put_values(std::move(ret)); }
    void return_value(RetType const &ret) { m_Result.put_values(ret); }

    RetType result()
    {
        if (m_ExceptionPtr) [[unlikely]] {
            std::rethrow_exception(m_ExceptionPtr);
        }

        return m_Result.move_value();
    }
};

template <>
struct Promise<void> {
    std::coroutine_handle<> m_Previous = nullptr;
    std::exception_ptr      m_ExceptionPtr{};


    Promise &operator=(Promise &&) = delete;

    std::coroutine_handle<Promise> get_return_object() { return std::coroutine_handle<Promise>::from_promise(*this); }

    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return PreviousAwaiter(m_Previous); /* handle sub coroutine*/ }
    auto unhandled_exception() { m_ExceptionPtr = std::current_exception(); }
    // auto yield_value() { m_ExceptionPtr = nullptr; }
    void return_void() { m_ExceptionPtr = nullptr; }

    void result()
    {
        if (m_ExceptionPtr) [[unlikely]] {
            std::rethrow_exception(m_ExceptionPtr);
        }
    }
};


template <typename T = void, class P = Promise<T>>
struct Task {
    using promise_type = P;

    // curent
    std::coroutine_handle<promise_type> m_Coroutine;

    // 35 rule? and the compiler requires it...
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
        T await_resume() const noexcept { return m_Coroutine.promise().result(); }
    };
    auto operator co_await() const { return Awaiter(m_Coroutine); }
    operator std::coroutine_handle<>() const { return m_Coroutine; }
};
