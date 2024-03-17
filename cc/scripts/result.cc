

#include <iostream>
#include <sys/cdefs.h>
#include <system_error>
#include <type_traits>
#include <variant>

template <class __Ok, class __Err>
class Result
{
  public:
    Result(__Ok ok) : var(ok) {}
    Result(__Err err) : var(err) {}
    Result<__Ok, __Err> operator=(__Ok ok) { return Result(ok); }
    Result<__Ok, __Err> operator=(__Ok &&ok) { return Result(std::move(ok)); }

    operator __Ok()
    {
        return std::get<__Ok>(var);
    }
    operator __Err() { return std::get<__Err>(var); }


    auto is_ok() -> bool { return std::holds_alternative<__Ok>(var); }
    auto unwrap() -> __Ok { return std::get<__Ok>(var); }
    auto unwrap_or(__Ok candidate) -> __Ok { return is_ok() ? std::get<__Ok>(var) : candidate; }

  private:
    std::variant<__Ok, __Err> var;
};



template <class __Ok>
struct Ok {
    Ok(__Ok ok) : var(ok){};

    Result<__Ok, class T> var;
};



auto f1() -> Result<int, const char *>
{
    return "hello";
    Ok(1);
}


int main()
{
    std::variant<int, float> v;
    v = 1;
    v = 1.f;

    std::cout << std::holds_alternative<int>(v) << std::endl;

    Result<int, double> v1(123.f);


    v1.unwrap();

    f1().unwrap();


    return 0;
}
