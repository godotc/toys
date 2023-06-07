#pragma once

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_timer.h"
#include "SDL_video.h"

#include <SDL2/SDL.h>
#include <cmath>
#include <glad/glad.h>

#include <cstdint>
#include <log.h>
#include <stdexcept>

#include <iostream>

namespace {
inline void ShowSDLError()
{
    std::cerr << SDL_GetError() << '\n';
}
}; // namespace

class Application
{
  public:
    Application() : Application("App", 800, 600, 0)
    {
    }

    Application(auto name, auto w, auto h, auto window_flags)
    {
        if (0 != SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Failed to init SDL Layer");
        }

        m_Width  = w;
        m_Height = h;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        // Set core profile or compatibility profile
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

        window_flags |= SDL_WINDOW_OPENGL;

        // Create Winodw
        SDL_Window *window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              m_Width, m_Height, window_flags);

        // or
        // SDL
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

        m_Window = window;
        m_GL_Ctx = gl_ctx;

        glViewport(0, 0, m_Width, m_Height);

        Construct();
    }

    virtual ~Application() = default;

  public:
    void Run()
    {
        BeginPlay();

        static uint32_t last_frame = SDL_GetTicks();


        while (m_IsRunning) {
            uint32_t cur_frame = SDL_GetTicks();
            uint32_t dleta     = cur_frame - last_frame;
            last_frame         = cur_frame;

            OnEvent();

            Tick(dleta);

            SDL_GL_SwapWindow(m_Window);
        }

        SDL_GL_DeleteContext(m_GL_Ctx);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }


  protected:

    virtual void Construct()
    {
        LOG("Base Construct..");
    }

    virtual void BeginPlay()
    {
        LOG("Base BeginPlay..");
    }

    virtual void Tick(uint32_t DeltaT)
    {
        // LOG("Base Tick..");
    }

    virtual void OnEvent()
    {
        // LOG("Base OnEvent..");
        SDL_Event ev;
        SDL_PollEvent(&ev);
        if (ev.type == SDL_QUIT) {
            m_IsRunning = false;
        }
        if (ev.type == SDL_KEYDOWN) {
            if (ev.key.keysym.sym == SDLK_ESCAPE) {
                m_IsRunning = false;
            }
        }
    }

  public:
    int m_Width{0}, m_Height{0};

    SDL_Window   *m_Window;
    SDL_GLContext m_GL_Ctx;


  protected:
    using Super = Application;

  private:
    bool m_IsRunning{true};
};
