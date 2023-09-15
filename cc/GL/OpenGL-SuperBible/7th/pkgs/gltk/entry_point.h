#pragma once



#include "GLFW/glfw3.h"
#include "window/sdl_window.h"
#include "window/window.h"
#include <cassert>
#include <cstdlib>
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


#include <memory>
#include <optional>
#include <stdexcept>

#include <iostream>
#include <variant>

namespace {
inline void ShowGlfwError()
{
    const char *error;
    glfwGetError(&error);
    std::cerr << error << '\n';
}
}; // namespace


using UpdateCallback = void(int dt);


enum class WindowBackend
{
    GLFW = 0,
    SDL  = 1,
};

class Entrypoint
{
  public:
    static Entrypoint &New(std::string program_name)
    {
        static Entrypoint app;
        app.program_name = program_name;
        return app;
    }

    Entrypoint &size(size_t w, size_t h)
    {
        m_Width  = w;
        m_Height = h;
        return *this;
    }

    Entrypoint &sdl_window(unsigned int sdl_window_flags)
    {
        if (!window_backend.has_value()) {
            LOG_ERROR("You has initialize windoww with {}", window_backend == WindowBackend::SDL ? "SDL" : "GLFW");
            exit(-1);
        }
        this->window_backend = WindowBackend::SDL;


        window = std::unique_ptr<window::Window>(
            new Window_SDL(program_name, sdl_window_flags, m_Width, m_Height, m_IsRunning));

        return *this;
    }

    Entrypoint &glfw_window()
    {

        if (!window_backend.has_value()) {
            LOG_ERROR("You has initialize windoww with {}", window_backend == WindowBackend::SDL ? "SDL" : "GLFW");
            exit(-1);
        }
        this->window_backend = WindowBackend::GLFW;

        return *this;
    }


    Entrypoint &init()
    {

        glViewport(0, 0, m_Width, m_Height);

        const GLubyte *bytes = glGetString(GL_VERSION);
        std::cout << "GL verison: " << bytes << std::endl;
        return *this;
    }


    void run()
    {
        window->run();
    }

    void shut_down()
    {
        m_IsRunning = false;
    }

  protected:

    void render(float dt);
    void updaet(float dt);

  private:

    Entrypoint()          = default;
    virtual ~Entrypoint() = default;

    void on_event(float dt)
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

  protected:
    using Super = Entrypoint;

  private:
    std::string                  program_name;
    std::optional<WindowBackend> window_backend;

    std::unique_ptr<window::Window> window;


    bool m_IsRunning{true};
};
