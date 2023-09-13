#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <log.h>


#include <cmath>
#include <cstddef>

#include <cstdint>


#include <stdexcept>

#include <iostream>

namespace {
inline void ShowSDLError()
{
    std::cerr << glGetError() << '\n';
}
}; // namespace

class App
{
  public:
    App() : App("App", 800, 600, 0)
    {
    }

    App(auto name, auto w, auto h, auto window_flags)
    {
        if (GLFW_TRUE != glfwInit()) {
            throw std::runtime_error("Failed to init glfw");
        }

        m_Width  = w;
        m_Height = h;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


        // Create Winodw
        auto *window = glfwCreateWindow(m_Width, m_Height, "glfw", nullptr, nullptr);
        if (!window) {
            throw std::runtime_error("failed to create winodws");
        }
        glfwMakeContextCurrent(window);


        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            ShowSDLError();
            throw std::runtime_error("Failed to initialize GLAD!!");
        }



        m_Window = window;

        glViewport(0, 0, m_Width, m_Height);

        Construct();
        glfwSetKeyCallback(m_Window, OnKeyEvent);
    }

    virtual ~App() = default;

  public:
    void Run()
    {
        BeginPlay();

        static size_t last_frame = glfwGetTime();


        while (!glfwWindowShouldClose(m_Window)) {
            size_t cur_frame = glfwGetTime();
            size_t dleta     = cur_frame - last_frame;
            last_frame       = cur_frame;

            glfwPollEvents();

            Tick(dleta);

            glfwSwapBuffers(m_Window);
        }

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }


  protected:
    virtual void Construct()
    {
        LOG_DEBUG("Base Construct..");
    }

    virtual void BeginPlay()
    {
        LOG_DEBUG("Base BeginPlay..");
    }

    virtual void Tick(uint32_t DeltaT)
    {
        // LOG("Base Tick..");
    }

    static void OnKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

  public:
    int m_Width{0},
        m_Height{0};

    GLFWwindow *m_Window;


  protected:
    using Super = App;

  private:
    bool m_IsRunning{true};
};
