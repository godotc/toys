


#include <chrono>
#include <coroutine>
#include <cstddef>
#include <exception>
#include <span>
#include <tuple>
#include <utility>
#include <variant>

#include "helpers.hpp"
#include "schelder.hpp"
#include "type_list.hpp"
#include "ut.hpp"

using namespace std::chrono_literals;

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


template <typename T = void>
struct Task {
    using promise_type = Promise<T>;

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



struct SleepAwaiter {
    std::chrono::system_clock::time_point m_ExperiedTime;

    bool await_ready() const noexcept { return std::chrono::system_clock::now() >= m_ExperiedTime; }

    std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
    {
        // synchornous way
        // std::this_thread::sleep_until(m_ExperiedTime);
        // return coroutine;

        GetScheduler().AddTimer(m_ExperiedTime, coroutine);
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


struct ReturnPreviousPromise {
    std::coroutine_handle<> m_Previous{};

    ReturnPreviousPromise &operator=(ReturnPreviousPromise &&) = delete;

    auto initial_suspend() noexcept { return std::suspend_always(); }
    auto final_suspend() noexcept { return PreviousAwaiter(m_Previous); }
    void unhandled_exception() { throw; }
    void return_value(std::coroutine_handle<> previous) noexcept { m_Previous = previous; }
    auto get_return_object() { return std::coroutine_handle<ReturnPreviousPromise>::from_promise(*this); }
};
struct ReturnPreviousTask {
    using promise_type = ReturnPreviousPromise;

    std::coroutine_handle<promise_type> m_Corutine;

    ReturnPreviousTask(std::coroutine_handle<promise_type> coroutine) noexcept
        : m_Corutine(coroutine) {}

    ReturnPreviousTask(ReturnPreviousTask &&) = delete;

    ~ReturnPreviousTask() { m_Corutine.destroy(); }
};


// clang-format off
namespace detail { 
	struct WhenAllControlBlock {
		std::size_t             m_Count{};
		std::coroutine_handle<> m_Previous{};
		std::exception_ptr      m_ExceptionPtr{};
	};

	struct WhenAllAwaiter {

		WhenAllControlBlock                &m_Control;
		std::span<ReturnPreviousTask const> m_Tasks;

		bool await_ready() const noexcept
		{
			return false; // block
		}

		// call on initialized? -> construct  WhenAllAwaiter(....) will do this
		std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const 
		{
			if (m_Tasks.empty()) {
				return coroutine;
			}
			m_Control.m_Previous = coroutine;
			for (auto const &t : m_Tasks) { // 1....n
				GetScheduler().AddTask(t.m_Corutine);
			}
			return m_Tasks.front().m_Corutine;
		}
		constexpr void await_resume() const 
		{
			if (m_Control.m_ExceptionPtr) [[unlikely]] {
				std::rethrow_exception(m_Control.m_ExceptionPtr);
			}
		}
	};


	template <typename T>
	ReturnPreviousTask when_all_helper(Task<T> const &t, WhenAllControlBlock &control, Uninitialized<T> &result)
	{
		try {
			result.put_values(co_await t);
		}
		catch (...) {
			control.m_ExceptionPtr = std::current_exception();
			co_return control.m_Previous;
		}

		--control.m_Count;
		if (control.m_Count == 0) {
			co_return control.m_Previous;
		}
		co_return nullptr;
	}


	template <std::size_t... Is, typename... Ts>
	Task<std::tuple<typename AwaitableTraits<Ts>::NonVoidRetType...>>
	when_all_impl(std::index_sequence<Is...>, Ts &&...ts)
	{
		WhenAllControlBlock control{
			.m_Count = sizeof...(Ts),
		};
		std::tuple<Uninitialized<typename AwaitableTraits<Ts>::RetType>...> result;

		ReturnPreviousTask tasks[]{when_all_helper(ts, control, std::get<Is>(result))...};
		co_await WhenAllAwaiter(control, tasks);
		co_return std::tuple<typename AwaitableTraits<Ts>::NonVoidRetType...>(
			std::get<Is>(result).move_value()...);
	}
} // namespace detail
// clang-format on


template <Awaitable... Ts>
    requires(sizeof...(Ts) != 0)
auto when_all(Ts &&...ts)
{
    return detail::when_all_impl(
        std::make_index_sequence<sizeof...(Ts)>{},
        std::forward<Ts>(ts)...);
}


// clang-format off
namespace detail { 
	struct WhenAnyControlBlock {
		static constexpr std::size_t null_index = std::size_t(-1);
		std::size_t m_Index = null_index;

		std::coroutine_handle<> m_Previous{};
		std::exception_ptr      m_ExceptionPtr{};
	};

	struct WhenAnyAwaiter {
		WhenAnyControlBlock                &m_Control;
		std::span<ReturnPreviousTask const> m_Tasks;

		bool await_ready() const noexcept
		{
			return false; // block
		}

		// call on initialized? -> construct  WhenAllAwaiter(....) will do this
		std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const 
		{
			if (m_Tasks.empty()) {
				return coroutine;
			}
			m_Control.m_Previous = coroutine;
			for (auto const &t : m_Tasks.subspan(1)) { // 1....n
				GetScheduler().AddTask(t.m_Corutine);
			}
			return m_Tasks.front().m_Corutine;
		}
		constexpr void await_resume() const noexcept
		{
			if (m_Control.m_ExceptionPtr) [[unlikely]] {
				std::rethrow_exception(m_Control.m_ExceptionPtr);
			}
		}
	};


	template <typename T>
	ReturnPreviousTask when_any_helper(Task<T> const &t, WhenAnyControlBlock &control, Uninitialized<T> &result, std::size_t index)
	{
		try {
			result.put_values(co_await t);
		}
		catch (...) {
			control.m_ExceptionPtr = std::current_exception();
			co_return control.m_Previous;
		}

		--control.m_Index = index;
		// return if any has complete (co_await complete)
		co_return control.m_Previous;
	}


	template <std::size_t... Is, typename... Ts>
	Task<std::variant<typename AwaitableTraits<Ts>::NonVoidRetType...>>
	when_any_impl(std::index_sequence<Is...>, Ts &&...ts)
	{
		WhenAnyControlBlock control;
		std::tuple<Uninitialized<typename AwaitableTraits<Ts>::RetType>...> result;

		ReturnPreviousTask tasks[]={when_any_helper(ts, control, std::get<Is>(result), Is)...};
		// get the finish index
		co_await WhenAnyAwaiter(control, tasks);

		Uninitialized<std::variant<typename AwaitableTraits<Ts>::NonVoidRetType...>> mono_result;

		((control.m_Index == Is &&
			(mono_result.put_values(
					// special contruct function of variant, the first arg to specify the n type of variant
					std::in_place_index<Is>, std::get<Is>(result).move_value()
				),0
			)
		),...);


		co_return  mono_result.move_value();
	}
} // namespace detail
// clang-format on


template <Awaitable... Ts>
    requires(sizeof...(Ts) != 0)
auto when_any(Ts &&...ts)
{
    return detail::when_any_impl(
        std::make_index_sequence<sizeof...(Ts)>{},
        std::forward<Ts>(ts)...);
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
    co_return i + 1;
}
Task<int> hello2()
{
    auto i = co_await world();

    co_await sleep_for(2ms);
    debug(), "hello2";
    co_return i + 1;
}


Task<int> print_1()
{
    debug(), 1;

    debug(), __FUNCITON_NAME__;
    co_return 1;
}
Task<int> print_2()
{
    debug(), 2;
    debug(), __FUNCITON_NAME__;
    co_return 2;
}

Task<int> test_wait_all()
{
    debug(), "begin of ", __FUNCITON_NAME__;
    auto [i, j, k] = co_await when_all(print_1(), print_2(), print_2());

    co_return j + k;
}
Task<int> test_wait_any()
{
    debug(), "begin of ", __FUNCITON_NAME__;
    auto var = co_await when_any(print_1(), print_2(), print_2());
    debug(), "var index: ", var.index() + 1, "  OK";
    co_return std::get<0>(var);
}


int main()
{

    // normal tasks
    //{
    //    Task t  = hello();
    //    auto t2 = hello2();
    //    auto t3 = hello2();
    //    GetScheduler().AddTask(t);
    //    GetScheduler().AddTask(t2);
    //    GetScheduler().AddTask(t3);

    //    GetScheduler().RunAll();
    //    debug(), t.m_Coroutine.promise().result();
    //    debug(), t2.m_Coroutine.promise().result();
    //}

    // when all
    //{
    //    auto t = test_wait_all();
    //    GetScheduler().AddTask(t);
    //    GetScheduler().RunAll();
    //    debug(), t.m_Coroutine.promise().result();
    //}

    // when any
    {
        auto t = test_wait_any();
        GetScheduler().AddTask(t);
        GetScheduler().RunAll();
        debug(), t.m_Coroutine.promise().result();
    }


    return 0;
}
