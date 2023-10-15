#pragma once
#include <any>
#include <optional>
#include <stdexcept>
#include <variant>

namespace mtd {


template <class T>
struct Runable {
    virtual auto run() -> std::any = 0;
};

template <class T>
struct Drop {
    virtual auto drop() -> void = 0;
};

template <class __Ok, class __Err>
struct Result {

  public:
    Result(__Ok &&value) : var(value) {}
    Result(__Err &&error) : var(error) {}

    auto is_ok() -> bool const { return std::holds_alternative<__Ok>(var); }
    auto ok_value() const -> const __Ok & { return std::get<__Ok>(var); }
    auto err_value() const -> const __Err & { return std::get<__Err>(var); }
    auto unwarp() const -> __Ok &
    {
        if (!is_ok())
            throw std::runtime_error("Get a Result by error");
        return var;
    }
    auto unwarp_or(__Ok &&candidate) const -> const __Ok &
    {
        if (!is_ok())
            return candidate;
        return var;
    }
    auto unwarp_or_error(std::string &&msg) const -> const auto &
    {
        if (!is_ok())
            throw std::runtime_error(msg);
        return var;
    }


  private:
    std::variant<__Ok, __Err> var;
};

template <class __Ok, class __Err, class __Result = Result<__Ok, __Err>>
struct Ok {

    Ok(__Result res) : var(__Result){]};

    std::variant<__Ok, __Err> var;
};


}; // namespace mtd
