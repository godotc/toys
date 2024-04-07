#pragma once

#include "base.hpp"
#include <coroutine>
#include <exception>
#include <memory>
#include <optional>
#include <utility>
namespace co_async {



template <class T>
struct GeneratorPromise {
    std::coroutine_handle<> m_Previous;
    std::exception_ptr      m_ExceptionPtr;
    bool                    m_Final = false;
    Uninitialized<T>        m_Result;

    GeneratorPromise &operator=(GeneratorPromise &&) = delete;

    auto initial_suspend() noexcept { return std::suspend_always(); }
    auto final_suspend() noexcept { return PreviousAwaiter(m_Previous); }
    void unhandled_exception() noexcept
    {
        m_ExceptionPtr = std::current_exception();
        m_Final        = true;
    }
    auto yield_value(T &&ret)
    {
        m_Result.put_values(std::move(ret));
        return PreviousAwaiter(m_Previous);
    }
    auto yield_value(T const &ret)
    {
        m_Result.put_values(ret);
        return PreviousAwaiter(m_Previous);
    }
    void return_void() { m_Final = true; }

    auto get_return_object() { return std::coroutine_handle<GeneratorPromise>::from_promise(*this); }

    bool final()
    {
        if (m_Final) {
            if (m_ExceptionPtr) [[unlikely]] {
                std::rethrow_exception(m_ExceptionPtr);
            }
        }
        return m_Final;
    }

    T result() { return m_Result.move_value(); }
};

template <class T>
struct GeneratorPromise<T &> {
    std::coroutine_handle<> m_Previous;
    std::exception_ptr      m_ExceptionPtr;
    T                      *m_Result;

    GeneratorPromise &operator=(GeneratorPromise &&) = delete;

    auto initial_suspend() noexcept { return std::suspend_always(); }
    auto final_suspend() noexcept { return PreviousAwaiter(m_Previous); }
    void unhandled_exception() noexcept
    {
        m_ExceptionPtr = std::current_exception();
        m_Result       = nullptr;
    }
    auto yield_value(T const &ret)
    {
        m_Result = std::addressof(ret);
        return PreviousAwaiter(m_Previous);
    }
    void return_void() { m_Result = nullptr; }

    auto get_return_object() { return std::coroutine_handle<GeneratorPromise>::from_promise(*this); }

    bool final()
    {
        if (!m_Result) {
            if (m_ExceptionPtr) [[unlikely]] {
                std::rethrow_exception(m_ExceptionPtr);
            }
            return true;
        }
        return false;
    }

    T &result() { return *m_Result; }
};



template <class T, class P = GeneratorPromise<T>>
class [[nodiscard]] Generator
{
    std::coroutine_handle<P> m_Coroutine;

  public:
    using promise_type = P;

    Generator(std::coroutine_handle<promise_type> coroutine = nullptr) noexcept : m_Coroutine(coroutine) {}
    Generator(Generator &&other) noexcept : m_Coroutine(other.m_Coroutine) { other.m_Coroutine = nullptr; }
    Generator operator&=(Generator &&other) noexcept { std::swap(m_Coroutine, other.m_Coroutine); }
    ~Generator()
    {
        if (m_Coroutine) {
            m_Coroutine.destroy();
        }
    }

    struct Awaiter {
        std::coroutine_handle<promise_type> m_Coroutine;

        bool await_ready() const noexcept { return false; }

        std::coroutine_handle<promise_type>
        await_suspend(std::coroutine_handle<> coroutine) const noexcept
        {
            m_Coroutine.promise().m_Previous = coroutine;
            return m_Coroutine;
        }

        std::optional<T> await_resume() const
        {
            if (m_Coroutine.promise().final()) {
                return std::nullopt;
            }
            return m_Coroutine.promise().result();
        }
    };
    auto operator co_await() const noexcept { return Awaiter(m_Coroutine); }
};

} // namespace co_async
