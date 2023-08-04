#pragma once
#include <any>
#include <optional>

namespace mtd {

const bool Ok  = true;
const bool Err = false;

template <class T>
struct Runable
{
    virtual auto run() -> std::any = 0;
};

template <class T>
struct Drop
{
    virtual auto drop() -> void = 0;
};

template <class Ok, class Err>
struct Result
{

  public:
    Result(Ok &&value) : _value(value), _isError(true) {}
    Result(Err &&error) : _error(error), _isError(false) {}
    auto is_ok() -> bool const { return !_isError; }
    auto ok_value() const -> const Ok & { return _value; }
    auto err_value() const -> const Err & { return _error; }
    auto unwarp() const -> const auto & { return _isError ? _error : _value; }


  private:
    Ok   _value;
    Err  _error;
    bool _isError;
};



}; // namespace mtd