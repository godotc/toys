#include <bits/types/struct_timeval.h>
#include <error.h>
#include <stdio.h>
#include <sys/time.h>

int main()
{

    itimerval tv{
        {2, 0},
        {2, 0}
    };

    auto ret = setitimer(ITIMER_REAL, &tv, nullptr);
    if (-1 == ret) {
        puts("failed");
    }

    getchar();
}
