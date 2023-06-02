#include "game.h"
#include "log.h"

#include <bit>
#include <cstdio>
#include <cstdlib>

#include "resource_manager/resource_manager.h"
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <functional>
#include <glm/common.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "render/sprite_render.h"

#include <format>

#include <GL/gl.h>

void framebuffer_resize_cb(auto *wndow, int w, int h);
void key_cb(auto *window, int key, int scancode, int action, int mode);



const unsigned int WIN_W = 800, WIN_H = 600;

static std::unique_ptr<Game> Breakout;



[[nodiscard]] GLFWwindow *init_glfw()
{
    if (GLFW_FALSE == glfwInit()) {
        throw std::runtime_error("init GLFW failed!!");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window = glfwCreateWindow(WIN_W, WIN_H, "Breakout", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    // glad hook funtions
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("Failed to initalize GLAD");
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_resize_cb);
    glfwSetKeyCallback(window, key_cb);

    // OpenGL configuration
    glViewport(0, 0, WIN_W, WIN_H);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const GLubyte *bytes = glGetString(GL_VERSION);
    std::cout << bytes << '\n';


    return window;
}

int main(int argc, char **argv)
{
    auto window = init_glfw();

    Breakout = std::make_unique<Game>(WIN_W, WIN_H);
    Breakout->Init();

    float delta_time = 0.f;
    float last_frame = 0.f;


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float current_frame = glfwGetTime();
        delta_time          = current_frame - last_frame;
        last_frame          = current_frame;

        Breakout->ProcessInput(delta_time);
        Breakout->Update(delta_time);

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        Breakout->Render();


        glfwSwapBuffers(window);
    }

    ResourceManager::Clear();

    glfwTerminate();

    return 0;
}



void framebuffer_resize_cb(auto *wndow, int w, int h)
{
    glViewport(0, 0, w, h);
}

void key_cb(auto *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key > 0 && key <= 1024) {
        if (action == GLFW_PRESS) {
            Breakout->m_keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            Breakout->m_keys[key] = false;
        }
    }
}
