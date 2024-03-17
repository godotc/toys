#pragma once
#include <tuple>
#include <type_traits>


namespace detail {

template <class FuncSig>
struct basic_function_traits;

template <class Ret, class... Args>
struct basic_function_traits<Ret (*)(Args...)> {
    using return_type = Ret;
    using args        = std::tuple<Args...>;
};

}; // namespace detail



template <class FuncSig>
struct function_traits;

template <typename Ret, typename... Args>
struct function_traits<Ret(Args...)> : detail::basic_function_traits<Ret(Args...)> {
    using type                      = Ret(Args...);
    using args_with_class           = std::tuple<Args...>; // remove class in declaration?
    using pointer                   = Ret (*)(Args...);    // function ptr
    static constexpr bool is_member = false;
    static constexpr bool is_const  = false;
};

template <typename Ret, typename Class, typename... Args>
struct function_traits<Ret (Class::*)(Args...)> : detail::basic_function_traits<Ret(Args...)> {
    using type                      = Ret (Class::*)(Args...);
    using args_with_class           = std::tuple<Class *, Args...>;
    using pointer                   = Ret (Class::*)(Args...); // function ptr
    static constexpr bool is_member = true;
    static constexpr bool is_const  = false;
};

template <typename Ret, typename Class, typename... Args>
struct function_traits<Ret (Class::*)(Args...) const> : detail::basic_function_traits<Ret(Args...)> {
    using type                      = Ret (Class::*)(Args...);
    using args_with_class           = std::tuple<Class *, Args...>;
    using pointer                   = Ret (Class::*)(Args...); // function ptr
    static constexpr bool is_member = true;
    static constexpr bool is_const  = true;
};