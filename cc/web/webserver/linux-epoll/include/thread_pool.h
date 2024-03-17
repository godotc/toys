#include <future>
#include <iostream>
#include <mutex>

class ThreadPool
{

    ThreadPool()
    {
    }

    void run()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
    }


  private:
    std::mutex m_Mutex;
    int        m_ThreadCount{1};
};
