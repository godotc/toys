#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <exception>



SDL_Window   *g_pWindow   = 0;
SDL_Renderer *g_pRenderer = 0;


int main()
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
        g_pWindow = SDL_CreateWindow("Chapter 1: Setting up SDL",
                                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     800, 600,
                                     SDL_WINDOW_SHOWN);
        if (g_pWindow != nullptr) {
            g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
        }
    }
    else {
        throw std::current_exception();
    }

    SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);

    // cleare the window to black
    SDL_RenderClear(g_pRenderer);

    // show the window
    SDL_RenderPresent(g_pRenderer);
    SDL_Delay(5000);
    SDL_Quit();



    return 0;
}
