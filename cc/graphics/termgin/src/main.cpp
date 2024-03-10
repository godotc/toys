#include "terminal_mode.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>


/**
 * @brief Get the init value by rotation object
 *
 * @param mat_w  matrix width
 * @param mat_h  matrix height
 * @param r  0-> 1->45 2->90 3->135
 * @return int
 */
int get_matrix_top_left_raw_index_by_rotation(int mat_w, int mat_h, int r)
{
    r %= 4;
    switch (r) {
    case 0:
        return 0;
    case 1:
        return mat_w * (mat_h - 1);
    case 2:
        return mat_w * mat_h - 1;
    case 3:
        return mat_w - 1;
    }
    return 0;
}


/**
 * @brief
 * sample a 3x3 roate:
 *  0 1 2          6 3 0
    3 4 5  -> 45   7 4 1   left to right - 3
    6 7 8          8 5 2   top to down +1

           -> 90    x * -1
                    y * -3
           -> 135   x * 3
                    y * 1
 * @param x
 * @param y
 * @param r
 * @return int
 */
int rotate_matrix_index(int x, int y, int r, int mat_w = 4, int mat_h = 4)
{
    r %= 4;
    int top_left_index = get_matrix_top_left_raw_index_by_rotation(mat_w, mat_h, r);


    switch (r % 4)
    {
    case 0:
        return y + x * mat_w;
    case 1:
        return top_left_index +
               -mat_w * x * 1 * y;
    case 2:
        return top_left_index +
               -1 * x + -mat_h * y;
    case 3:
        return top_left_index +
               mat_w * x + -1 * y;
    }
    return 0;
}

auto roate = &rotate_matrix_index;



struct App {


    static const int map_width     = 12;
    static const int map_height    = 18;
    static const int screen_width  = 80;
    static const int screen_height = 30;

    std::string items[7];
    char        map[map_width * map_height];
    char        screen[screen_width * screen_height];
    bool        bGameOver = false;

    int current_item     = 0;
    int current_rotation = 0;
    int current_x        = map_width / 2;
    int current_y        = 0;

    void init_shits();
    bool is_piece_fit(int item, int rot, int x, int y)
    {
        for (int px = 0; px < 4; ++px) {
            for (int py = 0; py < 4; ++py) {
                int item_i  = rotate_matrix_index(px, py, rot);
                int block_i = (y + py) * map_width + (x + px);

                if (x + px >= 0 && x + px < screen_width) {
                    if (y + py >= 0 && y + py < screen_height) {
                        if (items[item][item_i] == 'X' && map[block_i] != 0) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    App()
    {
        // init map
        // memset(map, 0, map_width * map_height);
        // memset(map + 0 * map_width, 9, map_width);
        // memset(map + (map_height - 1) * map_width, 9, map_width);
        // for (int i = 1; i < map_height - 1; ++i) {
        //     map[i * map_height] = map[(i + 1) * map_height - 1] = 9;
        // }

        for (int y = 0; y < map_height; y++)
        {
            for (int x = 0; x < map_width; x++)
            {
                map[y * map_width + x] = (x == 0 || x == map_width - 1 || y == map_height - 1) ? 9 : 0;
            }
        }

        // init screen
        memset(screen, ' ', screen_width * screen_height);


        // init misc
        init_shits();
    }

    void run()
    {
        TerminalMode mode;

        while (!bGameOver) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            // sleep(1);
            on_update();
            on_renderer();
        }
    }

    auto to_screen(int x, int y) { return (y + 2) * screen_width + (x + 2); }

    void on_update()
    {
        // iterate the [map] no screen
        for (int y = 0; y < map_height; ++y) {
            for (int x = 0; x < map_width; ++x) {
                auto map_loc    = y * map_width + x;
                auto screen_loc = to_screen(x, y);

                static const std::string blocks = " ABCDEFG=#";
                screen[screen_loc]              = blocks[map[map_loc]];
            }
        }

        // draw current tetromino item
        for (int px = 0; px < 4; ++px) {
            for (int py = 0; py < 4; ++py) {
                if (items[current_item][rotate_matrix_index(px, py, current_rotation)] == 'X') {
                    screen[to_screen(current_x + px, current_y + py)] = current_item + 65; // TO ABCD
                }
            }
        }
    }

    void on_renderer()
    {
        // draw screen
        for (int y = 0; y < screen_height; y++) {
            for (int x = 0; x < screen_width; x++) {
                putchar(screen[y * screen_width + x]);
            }
            putchar('\n');
        }
    }
};

int main(int argc, char **argv)
{

    App app;

    app.run();

    return 0;
}

inline void App::init_shits()
{
    {
        items[0] = ("..X."
                    "..X.."
                    ".X.."
                    ".X.");
        items[1] = ("..X."
                    ".XX.."
                    ".X.."
                    "...");
        items[2] = ("...."
                    ".XX.."
                    "XX..."
                    "..");
        items[3] = ("..X."
                    ".XX.."
                    "X...."
                    "..");
        items[4] = (".X.."
                    ".XX.."
                    ".X..."
                    "..");
        items[5] = (".X.."
                    ".X..."
                    "XX.."
                    "...");
        items[6] = ("..X."
                    "..X.."
                    "XX.."
                    "...");
    }
}
