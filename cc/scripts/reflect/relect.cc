

#include <chrono>
#include <string>
#include <tuple>
#include <utility>

#include "function_traits.hpp"
#include "variable_traits.hpp"


struct Person final {
    std::string last_name;
    float       height;
    // bool        is_transaxle;
    bool is_female;

    void Sleep() const {}
    // bool IsTransaxle() const { return is_transaxle; }
    bool IsFemale() const { return is_female; }
    bool GetMarriedWith(Person &other)
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

template <class T>
struct TypeInfo;


template <typename Ret, typename... Args>
auto function_pointer_type(Ret (*)(Args...)) -> Ret (*)(Args...);
template <typename Ret, typename Class, typename... Args>
auto function_pointer_type(Ret (Class::*)(Args...)) -> Ret (Class::*)(Args...);
template <typename Ret, typename Class, typename... Args>
auto function_pointer_type(Ret (Class::*)(Args...) const) -> Ret (Class::*)(Args...) const;

template <auto FunctionVal>
using function_pointer_type_t = decltype(function_pointer_type(FunctionVal));

template <>
struct TypeInfo<Person> {
    using functions = std::tuple<
        function_traits<decltype(&Person::Sleep)>,
        function_traits<function_pointer_type_t<&Person::IsFemale>>>;
};


int main()
{
    using a      = function_pointer_type_t<&Person::IsFemale>;
    auto b       = function_pointer_type(&Person::Sleep);
    using name_t = variable_traits<decltype(&Person::last_name)>;
    using func_t = variable_traits<decltype(&Person::GetMarriedWith)>;
}