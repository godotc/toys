#pragma once

#include <log.h>

#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_timer.h>
#include <SDL_video.h>


#include <cmath>
#include <cstddef>
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

class App
{
  public:
    App() : App("App", 800, 600, 0)
    {
    }

  protected:

    virtual void init(){};
    virtual void update(float dt){};
    virtual void render(float dt){};

  public:

    App(auto name, auto w, auto h, auto window_flags)
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
    }

    virtual ~App() = default;

  public:
    void Run()
    {
        init();

        while (m_IsRunning) {
            current_frame = SDL_GetTicks();
            size_t dt     = current_frame - last_frame;
            last_frame    = current_frame;

            on_event();

            update(dt);
            render(dt);

            SDL_GL_SwapWindow(m_Window);
        }

        SDL_GL_DeleteContext(m_GL_Ctx);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }

    void Shutdown()
    {
        m_IsRunning = false;
    }

  protected:


    void on_event()
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

    size_t        current_frame, last_frame;
    SDL_Window   *m_Window;
    SDL_GLContext m_GL_Ctx;


  protected:
    using Super          = App;
    size_t tick_interval = 1;

  private:
    bool m_IsRunning{true};
};
