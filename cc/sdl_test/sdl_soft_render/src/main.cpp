#include <SDL2/SDL.h>
#include <iostream>

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Software Renderer",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return 1;
    }


    // Create a texture to hold the image
    SDL_Texture *texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STATIC,
                                             WINDOW_WIDTH, WINDOW_HEIGHT);
    if (texture == nullptr) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a pixel buffer for the image
    Uint32 pixels[WINDOW_WIDTH * WINDOW_HEIGHT];
    // Event loop
    bool quit = false;

    SDL_Event event;
    while (!quit)
    {
        for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; ++i) {
            pixels[i] = 0xFF000000; // Black color
        }

        // Set the pixel buffer as the texture data
        SDL_UpdateTexture(texture, nullptr, pixels, WINDOW_WIDTH * sizeof(Uint32));

        // Clear the renderer
        SDL_RenderClear(renderer);

        // Copy the texture to the renderer
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        // Render to the window
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
