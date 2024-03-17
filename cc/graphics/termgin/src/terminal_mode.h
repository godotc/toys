#pragma once
#include "signal.h"
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

struct TerminalMode {

    static termios &get_origin_cfg()
    {
        static auto origin_cfg = termios();
        return origin_cfg;
    }

    TerminalMode()
    {
        tcgetattr(STDIN_FILENO, &get_origin_cfg());

        auto target_cfg = get_origin_cfg();
        // disbale canonical mdoe and echoing target_cfg.c_lflag &= ~(ICANON | ECHO);

        //  set timeout for read return immediately
        target_cfg.c_cc[VTIME] = 0;
        target_cfg.c_cc[VMIN]  = 1;

        // apply but don't know wtf is the args
        tcsetattr(STDIN_FILENO, TCSANOW, &target_cfg);

        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

        if (signal(SIGINT, on_interup) == SIG_ERR) {
            fprintf(stderr, "Failed to register the Ctrl+C signal handler.\n");
            tcsetattr(STDIN_FILENO, TCSANOW, &get_origin_cfg());
            exit(1);
        }
    }

    static void on_interup(int sig)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &get_origin_cfg());
        exit(-1);
    }

    ~TerminalMode()
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &get_origin_cfg());
    }
};