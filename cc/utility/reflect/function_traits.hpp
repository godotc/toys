#pragma once
#include <stdint.h>
#include <tuple>


namespace detail {

template <class FuncSig>
struct basic_function_traits;

template <class Ret, class... Args>
struct basic_function_traits<Ret(Args...)> {
    using return_type = Ret;
    using args        = std::tuple<Args...>;
    using parameters  = std::tuple<Args...>;
};


}; // namespace detail



template <typename Func>
struct function_traits;

template <typename Ret, typename... Args>
struct function_traits<Ret(Args...)>
    : detail::basic_function_traits<Ret(Args...)> {
    using type                      = Ret(Args...);
    using args_with_class           = std::tuple<Args...>;
    using pointer                   = Ret (*)(Args...);
    static constexpr bool is_member = false;
    static constexpr bool is_const  = false;
};

// class function
template <typename Ret, typename Class, typename... Args>
struct function_traits<Ret (Class::*)(Args...)>
    : detail::basic_function_traits<Ret(Args...)> {

    using type            = Ret (Class::*)(Args...);
    using args_with_class = std::tuple<Class *, Args...>;
    using pointer         = Ret (Class::*)(Args...);

    static constexpr bool is_member = true;
    static constexpr bool is_const  = false;
};

// class pure function
template <typename Ret, typename Class, typename... Args>
struct function_traits<Ret (Class::*)(Args...) const>
    : detail::basic_function_traits<Ret(Args...)> {

    using type            = Ret (Class::*)(Args...) const;
    using args_with_class = std::tuple<Class *, Args...>;
    using pointer         = Ret (Class::*)(Args...) const;

    static constexpr bool is_member = true;
    static constexpr bool is_const  = true;
};