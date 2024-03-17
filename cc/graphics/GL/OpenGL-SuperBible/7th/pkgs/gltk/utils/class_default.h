// Codes from zenovis
#pragma once


namespace utils {

struct disable_copy_and_move
{
    disable_copy_and_move() = default;

    disable_copy_and_move(disable_copy_and_move const &) = delete;
    disable_copy_and_move(disable_copy_and_move &&)      = delete;

    disable_copy_and_move &operator=(disable_copy_and_move const &)  = delete;
    disable_copy_and_move &operator=(disable_copy_and_move const &&) = delete;
};

struct disable_copy_allow_move
{
    disable_copy_allow_move() = default;

    disable_copy_allow_move(disable_copy_allow_move const &)            = delete;
    disable_copy_allow_move &operator=(disable_copy_allow_move const &) = delete;

    disable_copy_allow_move(disable_copy_allow_move &&) = default;
    // disable_copy_allow_move &operator=(disable_copy_allow_move const &&) = default;
};

struct explicit_default_constructor
{
    explicit explicit_default_constructor() = default;
};

} // namespace utils
