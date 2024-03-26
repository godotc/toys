

#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#if 0
    #include <vcruntime.h>
#endif


#include "srefl.hpp"
#include "type_list.hpp"


struct Person final {
    std::string last_name;
    float       height;
    // bool        is_transaxle;
    bool is_female;

    void Sleep() const
    {
        printf("Zzzzz....\n");
    }
    // bool IsTransaxle() const { return is_transaxle; }
    bool IsFemale() const
    {
        printf("???....\n");
        return is_female;
    }
    bool GetMarriedWith(Person other)
    {
        if (other.is_female != is_female) //&& is_transaxle != other.is_transaxle;)
        {
            if (is_female) {
                last_name = "Mrs." + other.last_name;
            }
            else {
                last_name = "Mr." + last_name;
            }
            return true;
        }
        return false;
    }
};



BEGIN_CLASS(Person)
functions(
    func(&Person::Sleep),
    func(&Person::IsFemale),
    func(&Person::GetMarriedWith));
END_CLASS();


template <size_t Index, typename... Args, typename Class>
void VisitTuple(const std::tuple<Args...> &tuple, Class *instance)
{
    using tuple_type = std::tuple<Args...>;

    if constexpr (Index > std::tuple_size_v<tuple_type>) {
        return;
    }

    if constexpr (auto elem = std::get<Index>(tuple);
                  elem.param_count() == 0) {
        if (instance && elem.pointer) {
            (instance->*elem.pointer)();
        }
        VisitTuple<Index + 1>(tuple, instance);
    }
}

template <size_t... Index, typename Tuple, typename Function>
void VisitTuple(Tuple tuple, Function &&func, std::index_sequence<Index...>)
{
    (func(std::get<Index>(tuple)), ...);
}


template <typename T>
struct change_to_float {
    using type = std::conditional_t<std::is_integral_v<T>, float, T>;
};

template <typename T>
struct only_integer {
    constexpr static bool value = std::is_integral_v<T>;
};


int main()
{
    // using type1 = function_pointer_type_t<&Person::GetMarriedWith>;

    // static_assert(std::is_same_v<type1, bool (Person::*)(Person &)>);

    auto field = field_traits{&Person::GetMarriedWith, "&Person::GetMarriedWith"};
    std::cout << field.pointer << std::endl;
    std::cout << field.is_member() << std::endl;
    std::cout << field.is_const() << std::endl;
    std::cout << (field.pointer == &Person::GetMarriedWith) << std::endl;

    auto field2 = field_traits{&Person::IsFemale, "&Person::GetMarriedWith"};
    std::cout << field2.is_const() << std::endl;


    // constexpr auto info = reflect_info<Person>();
    // std::cout << std::get<0>(info.functions).name << std::endl;


    // // Person p;
    // // VisitTuple<0>(info.functions, &p);
    // using tuple_type = decltype(info.functions);

    // VisitTuple(
    //     info.functions, [](auto &&elem) {
    //         std::cout << elem.name() << std::endl;
    //     },
    //     std::make_index_sequence<std::tuple_size_v<tuple_type>>());
    // auto ptr = &Person::Int

    // using type = type_list<int, char, bool, double>;

    // using first_elem = head_t<type>;
    // using the_2      = nth_t<type, 2>;

    // constexpr size_t num = count<type, std::is_integral>;
    // std::cout << num << std::endl;

    // using changed = detail::map<type, change_to_float>::type;

    // using filtered = filter<type, only_integer>;
}