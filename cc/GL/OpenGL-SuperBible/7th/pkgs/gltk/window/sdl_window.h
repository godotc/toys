
#pragma once

#include "entry_point.h"
#include "window/window.h"
#include <SDL2/SDL.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <log.h>


#include <glad/glad.h>

#include <cstdint>


#include <stdexcept>

#include <iostream>

namespace {
inline void ShowSDLError()
{
    std::cerr << SDL_GetError() << '\n';
}
}; // namespace



class Window_SDL : public window::Window
{
  public:

    Window_SDL(std::string program_name, unsigned int window_flags, int w, int h, bool &is_running)
        : m_IsRunning(is_running)
    {
        sdl_window(program_name.c_str(), window_flags, w, h);
    }

    void sdl_window(const char *program_name, unsigned int sdl_window_flags, int w, int h)
    {
        sdl_window_flags |= SDL_WINDOW_OPENGL;

        // Create Winodw
        SDL_Window *window = SDL_CreateWindow(program_name,
                                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              w, h, sdl_window_flags);

        SDL_GLContext gl_ctx = SDL_GL_CreateContext(window);
        if (gl_ctx == nullptr) {
            ShowSDLError();
            throw std::runtime_error("Get GL context failed");
        }


        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            ShowSDLError();
            throw std::runtime_error("Failed to initialize GLAD!!");
        }

        SDL_GL_MakeCurrent(window, gl_ctx);

        Window   = window;
        GLContex = gl_ctx;
    }

    void run(auto on_event, auto update, auto render)
    {
        static size_t last_frame = SDL_GetTicks();


        while (m_IsRunning) {
            size_t cur_frame = SDL_GetTicks();
            size_t dt        = cur_frame - last_frame;
            last_frame       = cur_frame;

            on_event(dt);
            update(dt);
            render(dt);

            SDL_GL_SwapWindow(Window);
        }

        SDL_GL_DeleteContext(GLContex);
        SDL_DestroyWindow(Window);
        SDL_Quit();
    }


  public:
    SDL_Window   *Window;
    SDL_GLContext GLContex;
    bool         &m_IsRunning;
};
