#pragma once


#include <algorithm>
#include <coroutine>
#include <memory>
#include <utility>

template <class T = void>
struct NonVoidHelper {
    using type = T;
};
template <>
struct NonVoidHelper<void> {
    using type = NonVoidHelper;

    explicit NonVoidHelper() = default;
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
struct Uninitialized<T &&> : Uninitialized<T> {};

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
