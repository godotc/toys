/**
 *  Author: @godot42
 *  Create Time: 2024-03-28 04:35:12
 *  Modified by: @godot42
 *  Modified time: 2024-03-28 04:50:57
 *  Description:
 */



#include "base.hpp"
#include "generator.hpp"
#include "generic_loop.h"
#include "ut.hpp"
#include <cerrno>
#include <chrono>
#include <coroutine>
#include <exception>

#include "condition.hpp"

#include "epoll_loop.hpp"
#include "timer_loop.hpp"

#include "ut.hpp"


#include <ostream>
#include <string>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <tuple>
#include <unistd.h>

using namespace std::chrono_literals;



Task<void> nobody()
{
    debug(), "nobody";
    co_return;
}
Task<int> world()
{
    debug(), "world";
    // throw std::runtime_error("world failed");
    // throw "world failed"; this cannot print what() by exception handler
    co_return 42;
}

Task<int> hello()
{
    auto i = co_await world();
    co_await nobody();

    co_await co_async::sleep_for(co_async::GetTimerLoop(), 1s);
    debug(), "hello";
    co_return i + 1;
}
Task<int> hello2()
{
    auto i = co_await world();

    co_await co_async::sleep_for(co_async::GetTimerLoop(), 2ms);
    debug(), "hello2";
    co_return i + 1;
}


Task<int> print_1()
{
    debug(), 1;
    debug(), __FUNCITON_NAME__;
    co_await co_async::sleep_for(co_async::GetTimerLoop(), 2s);
    co_return 1;
}
Task<int> print_2()
{
    debug(), 2;
    debug(), __FUNCITON_NAME__;
    co_await co_async::sleep_for(co_async::GetTimerLoop(), 2s);
    co_return 2;
}

Task<int> test_when_all()
{
    debug(), "begin of ", __FUNCITON_NAME__;
    auto [i, j, k] = co_await co_async::when_all(print_1(), print_2(), print_2());

    co_return j + k;
}
Task<int> test_when_any()
{
    debug(), "begin of ", __FUNCITON_NAME__;
    auto var = co_await co_async::when_any(print_1(), print_2());
    debug(), "???";
    debug(), "var index: ", var.index(), "  OK";
    // co_await co_async::sleep_for(co_async::GetTimerLoop(), 3s);
    co_return var.index();
}

void test_before_epolls()
{
    try {
        // normal tasks
        //{
        //    Task t  = hello();
        //    auto t2 = hello2();
        //    auto t3 = hello2();
        //    co_async::GetScheduler().AddTask(t);
        //    co_async::GetScheduler().AddTask(t2);
        //    co_async::GetScheduler().AddTask(t3);

        //    co_async::GetScheduler().RunAll();
        //    debug(), t.m_Coroutine.promise().result();
        //    debug(), t2.m_Coroutine.promise().result();
        //}

        // when all
        // auto t = test_wait_all();
        auto t = test_when_any();
        t.m_Coroutine.resume();
        while (!t.m_Coroutine.done()) {
            co_async::GetTimerLoop().Run();
        }
        debug(), "the test_wait_any task result: ", t.m_Coroutine.promise().result();
    }
    catch (std::exception &excep) {
        debug(), excep.what();
    }
}

//----------------------------------- Epolls

co_async::GenericLoop loop;

inline Task<std::string> reader(co_async::AsyncFile &f)
{
    // wait std input avaliable;

    std::string buf(1024, '0');
    auto        which = co_await co_async::when_any(co_async::read_file(loop, f, buf),
                                                    sleep_for(loop, 1s));
    if (which.index() != 0) {
        co_return "No input since 1 second";
    }
    auto len = std::get<0>(which);
    // buf[std::get<0>(which)] = '\0'
    co_return buf.substr(0, len); // whatever~~ for testing? or how should it be optimized?
}


Task<void> async_main()
{
    int attr = 1;

    auto f = co_async::AsyncFile(STDIN_FILENO);
    f.SetNonBlock();

    while (true) {
        auto s = co_await reader(f);
        debug(), "read: ", s;
        if (s == "quit\n") {
            debug(), "you have input that quit, so you just quit the async_main";
            break; // co_return;
        }
    }
}



void test_async_xxx_TODO()
{
    auto t = async_main();
    t.m_Coroutine.resume();

    while (!t.m_Coroutine.done())
    {
        loop.run();
    }
}

// ------------------------------------ Generators

co_async::Generator<int> gen()
{
    for (int i = 0; i < 42; ++i) {
        co_yield i;
    }
}

Task<> amain()
{
    auto g = gen();
    for (int i = 0; i < 52; ++i) {
        debug(), co_await g;
    }
    co_return;
}

void test_generator()
{
    auto t = amain();
    while (!t.m_Coroutine.done()) {
        t.m_Coroutine.resume();
    }
    debug(), t.m_Coroutine.promise().result();
}


int main()
{
    test_before_epolls(); // do not pass
    // test_async_xxx_TODO(); // passed
    // test_generator(); // TODO



    return 0;
}
