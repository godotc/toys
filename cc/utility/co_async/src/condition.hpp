
#pragma once



#include "base.hpp"
#include <concepts>
#include <coroutine>
#include <cstddef>
#include <memory>
#include <ranges>
#include <span>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace co_async {


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
			for (auto const &t : m_Tasks.subspan(0,m_Tasks.size()-1)) { // 1....n
				t.m_Corutine.resume();
			}
			return m_Tasks.back().m_Corutine;
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
			result.put_values(co_await std::forward<decltype(t)> (t));
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
		std::tuple<Uninitialized<typename AwaitableTraits<Ts>::NonVoidRetType>...> result;

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


template <Awaitable T, class Alloc = std::allocator<T>>
Task<std::conditional_t<std::same_as<void, typename AwaitableTraits<T>::RetType>,
                        std::vector<typename AwaitableTraits<T>::RetType, Alloc>,
                        void>>
when_all(std::vector<T, Alloc> const &tasks)
{
    detail::WhenAllControlBlock control{.m_Count = tasks.size()};

    Alloc alloc = tasks.get_allocator();

    std::vector<Uninitialized<typename AwaitableTraits<T>::RetType>, Alloc> results(tasks.size(), alloc);
    {
        std::vector<ReturnPreviousTask, Alloc> task_array(alloc);
        task_array.reserve(tasks.size());
        for (std::size_t i = 0; i < tasks.size(); ++i) {
            task_array.push_back(detail::when_all_helper(tasks[i], control, results[i]));
        }
        co_await detail::WhenAllAwaiter(control, task_array);
    }
    if constexpr (!std::same_as<void, typename AwaitableTraits<T>::RetType>)
    {
        std::vector<typename AwaitableTraits<T>::RetType, Alloc> res(alloc);
        res.reserve(tasks.size());
        for (auto &ret : results) {
            res.push_back(ret.move_value());
        }
        co_return res;
    }
}



//--------------------------------------------------



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
			for (auto const &t : m_Tasks.subspan(0,m_Tasks.size()-1)) // 1....n
			{ 
				// debug() , __FUNCITON_NAME__;
				// debug(), t.m_Corutine.promise().m_Previous.address();
				t.m_Corutine.resume();
			}
			return m_Tasks.back().m_Corutine;
		}
		void await_resume() const 
		{
			if (m_Control.m_ExceptionPtr) [[unlikely]] {
				std::rethrow_exception(m_Control.m_ExceptionPtr);
			}
		}
	};


	template <typename T>
	ReturnPreviousTask
	when_any_helper(
		auto &&t,
		WhenAnyControlBlock &control,
		Uninitialized<T> &result,
		std::size_t index)
	{
		try {
			result.put_values(
					( co_await std::forward<decltype(t)>(t), NonVoidHelper<void>{})
				);
		}
		catch (...) {
			control.m_ExceptionPtr = std::current_exception();
			co_return control.m_Previous;
		}

		control.m_Index = index;
		// return if any has complete (co_await complete)
		co_return control.m_Previous;
	}


	template <std::size_t... Is, typename... Ts>
	Task<std::variant<typename AwaitableTraits<Ts>::NonVoidRetType...>>
	when_any_impl(std::index_sequence<Is...>, Ts &&...ts)
	{
		WhenAnyControlBlock control;
		std::tuple<Uninitialized<typename AwaitableTraits<Ts>::NonVoidRetType>...> result;

		ReturnPreviousTask tasks[]={ when_any_helper(ts, control, std::get<Is>(result), Is)...};
		// get the finish index
		co_await WhenAnyAwaiter(control, tasks);

		Uninitialized<std::variant<typename AwaitableTraits<Ts>::NonVoidRetType...>> mono_result;

		(
		(control.m_Index == Is && 
			 (mono_result.put_values(
				// special contruct function of variant,
				// the first arg to specify the n type of variant
				std::in_place_index<Is>, std::get<Is>(result).move_value()),
			  0)
	 	),...
		);


		co_return  mono_result.move_value();
	}
} // namespace detail
// clang-format on


template <Awaitable... Ts>
    requires(sizeof...(Ts) != 0)
auto when_any(Ts &&...ts)
{
    return detail::when_any_impl(std::make_index_sequence<sizeof...(Ts)>{},
                                 std::forward<Ts>(ts)...);
}

template <Awaitable T, class Alloc = std::allocator<T>>
Task<typename AwaitableTraits<T>::RetType>
when_any(std::vector<T, Alloc> const &tasks)
{
    detail::WhenAnyControlBlock control;

    Alloc alloc = tasks.get_allocator();

    Uninitialized<typename AwaitableTraits<T>::RetType> result;
    {
        std::vector<ReturnPreviousTask, Alloc> task_array(alloc);
        task_array.reserve(tasks.size());
        for (auto &task : tasks) {
            task_array.push_back(detail::when_any_helper(task, control, result));
        }

        co_await detail::WhenAnyAwaiter(control, task_array);
    }
    co_return result.move_value();
}



} // namespace co_async
