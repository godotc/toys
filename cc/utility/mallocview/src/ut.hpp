#pragma once

#include <chrono>
#include <cstddef>
#include <iostream>
#include <optional>
#include <ostream>
#include <type_traits>
#include <utility>
#ifdef WIN32
    #define __FUCNTION_NAME__ __FUNCSIG__
#else
    #define __FUNCITON_NAME__ __PRETTY_FUNCTION__
#endif


template <class T>
inline std::ostream &operator<<(std::ostream &out, std::optional<T> v)
{
    if (v.has_value()) {
        out << v.value();
    }
    else {
        out << "[None]";
    }
    return out;
}


template <int Cur, std::size_t UpperBound, typename Fn, typename... Ts>
inline auto static_for(Fn func, Ts... ts)
{
    if constexpr (Cur < UpperBound) {
        func(std::integral_constant<int, Cur>{}, std::forward<Ts>(ts)...);
        static_for<Cur + 1, UpperBound>(func, std::forward<Ts>(ts)...);
    }
}

/*
template <class... Ts>
inline std::ostream &operator<<(std::ostream &out, std::variant<Ts...> v)
{

    bool ok = false;
    static_for<0, sizeof...(Ts)>([&ok, &v, &out](auto i) {
        if (ok) {
            return;
        }
        using t = std::variant_alternative_t<i.value, std::variant<Ts...>>;
        if (const t *val = std::get_if<t>(&v)) {
            // if constexpr (std::holds_alternative<i.value>(v)) {
            out << *val;
            ok = true;
        }
    });

    if (!ok) {
        out << "Invlaid variant...., index: ", v.index();
    }

    return out;
}

*/


struct debug {
    template <class T>
    debug &operator,(T arg)
    {
        std::cout << arg;
        return *this;
    }
    ~debug() { std::cout << '\n'; }
};
// static auto d = debug{};



inline std::chrono::seconds operator""_s(unsigned long long value)
{
    return std::chrono::seconds(value);
}


namespace co_async {

} // namespace co_async
