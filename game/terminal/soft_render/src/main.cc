#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fame_buffer.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <unistd.h>


int main()
{
    using toplevel_namespace::FrameBuffer;
    system("clear");

    FrameBuffer buf(30, 20);

    // hide cusor
    std::cout << "\033[?25l";

    unsigned int last_frame = time(nullptr);

    std::atomic<bool> bShouldEnd = false;

    std::thread t([&bShouldEnd]() {
        while (!bShouldEnd) {
            auto c = getc_unlocked(stdin);
            if (c == 'q') {
                bShouldEnd = true;
            }
        }
    });


    while (!bShouldEnd)
    {

        auto current_frame = time(nullptr);
        auto dieta_frame   = current_frame - last_frame;
        last_frame         = current_frame;

        if (1) {
            // if (dieta_frame % 20 == 0) {
            buf.CleanBuffer();

            int  i = time(nullptr) % 2;
            auto j = i + 10;
            for (; i < j; ++i)
            {
                buf.SetPixel(glm::vec3(i, i, i), '#');
            }

            buf.Display();
        }
    }

    // display cusor
    std::cout << "\033[?25h";

    t.join();

    return 0;
}
