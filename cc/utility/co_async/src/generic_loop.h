#pragma once

#include "epoll_loop.hpp"
#include "timer_loop.hpp"

namespace co_async {


class GenericLoop
{
    co_async::TimerLoop m_TimerLoop;
    co_async::EpollLoop m_EpollLoop;

  public:
    operator co_async::TimerLoop &() { return m_TimerLoop; }
    operator co_async::EpollLoop &() { return m_EpollLoop; }

    void run()
    {
        while (true) {
            auto delay     = m_TimerLoop.Run();
            bool has_event = m_EpollLoop.TryRun(delay);
            if (!delay && !has_event) {
                break;
            }
        }
    }
};
} // namespace co_async
