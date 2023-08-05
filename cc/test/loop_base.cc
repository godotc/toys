#include <cstdio>
#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>

class Base
{
  public:
    std::mutex   _mutex;
    bool         _run    = true;
    std::thread *_thread = nullptr;
    Base() { this->enter(this); }
    virtual ~Base()
    {
        puts("~Base");
        this->exit();
    }

    void exit()
    {
        if (this->_thread) {
            std::unique_lock<std::mutex> lock(_mutex);
            this->_run = false;
            this->_thread->join();
            delete this->_thread;
            this->_thread = nullptr;
        }
    }
    void enter(Base *b)
    {
        if (_thread == nullptr) {
            _thread = new std::thread([this](auto b) {
                this->loopBegin();
                this->loop();
                b->loopOver();
                // this->loopOver();
            },
                                      b);
        }
    }

    void loop()
    {
        while (true) {
            puts("OK");
            sleep(1);
            if (!this->_run) {
                break;
            }
        }
    }
    virtual void loopBegin() = 0;
    virtual void loopOver()  = 0;
    static void  doThread(Base *game)
    {
        game->loopBegin();
        game->loop();
        game->loopOver();
    }
};
class Game : public Base
{
  public:
    //  Game() { int a = 1; }
    void loopBegin() override
    {
        int begin = 1;
        begin += 100;
    }
    void loopOver() override
    {
        int over = 10;
        over += 10;
    }

    ~Game()
    {
        puts("~Game");
    }
};

int main()
{
    Base *ptr = new Game();
    //  ptr->enter();
    sleep(5);
    //  ptr->exit();

    delete ptr;
}
