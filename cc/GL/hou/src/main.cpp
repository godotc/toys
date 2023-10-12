
#include "resource_manager/resource_manager.h"
#include "utils/path.h"
#include <iostream>



#include "log.h"
#include <bit>
#include <cstdio>
#include <cstdlib>

#include "glad/glad.h"
#include "m_log/level.h"
#include "resource_manager/resource_manager.h"

#include <GLFW/glfw3.h>

#include <functional>
#include <glm/common.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "render/camera.h"

#include "gl_macros.h"

#include <glm/gtc/type_ptr.hpp>

void            framebuffer_resize_cb(GLFWwindow *wndow, int w, int h);
void            key_cb(GLFWwindow *window, int key, int scancode, int action, int mode);
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);



const unsigned int WIN_W = 800,
                   WIN_H = 600;


static GLFWwindow *init_glfw();


class App
{
  public:
    App()
    {
        GLfloat vertices[] = {
            // Front face
            -0.5f, -0.5f, 0.5f, // 0
            0.5f, -0.5f, 0.5f,  // 1
            0.5f, 0.5f, 0.5f,   // 2
            -0.5f, 0.5f, 0.5f,  // 3
            // Back face
            -0.5f, -0.5f, -0.5f, // 4
            0.5f, -0.5f, -0.5f,  // 5
            0.5f, 0.5f, -0.5f,   // 6
            -0.5f, 0.5f, -0.5f,  // 7
        };

        GLuint indices[] = {
            0, 1, 2, 0, 2, 3, // Front face
            1, 5, 6, 1, 6, 2, // Right face
            4, 7, 6, 4, 6, 5, // Back face
            0, 3, 7, 0, 7, 4, // Left face
            3, 2, 6, 3, 6, 7, // Top face
            0, 4, 5, 0, 5, 1  // Bottom face
        };

        GL_CHECK_HEALTH();

        glCreateVertexArrays(1, &cube_va);
        glBindVertexArray(cube_va);
        {
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)nullptr);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glBindVertexArray(0);
        GL_CHECK_HEALTH();

        GL_CALL(glGenBuffers(1, &cube_ib));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ib);
        GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

        GL_CHECK_HEALTH();

        ResourceManager::LoadShader(FPath("res/shader/cube.glsl"), "cube");

        ResourceManager::LoadTexture(FPath("res/texture/arch.png"), "arch");
        auto Texture = ResourceManager::GetTexture("arch");
    }

    ~App() {}

    void OnRenderer()
    {
        glm::mat4 viewmodel = glm::scale(glm::mat4(1.0), {100, 100, 100});

        ResourceManager::GetShader("cube").Use().SetMatrix4("viewmodel", viewmodel);

        glBindVertexArray(cube_va);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ib);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }
    void OnUpdate(float dt)
    {
    }

    void ProcessInput(float dt)
    {
    }

    Camera camera = {
        {0, 0, 3},
        {0, 0, 0},
        {0, 1, 0}
    };

    GLuint cube_va, cube_ib;
};



int main(int argc, char **argv)
{
    __logcpp::SetLogLevel(__logcpp::LogLevel::L_DEBUG);
    auto window = init_glfw();
    LOG_LOG("GLFW has initialize successfully");


    App *app = new App();



    float dt         = 0.f;
    float last_frame = 0.f;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float current_frame = glfwGetTime();
        dt                  = current_frame - last_frame;
        last_frame          = current_frame;
        // LOG_DEBUG("dt: {}", dt);

        app->OnRenderer();
        app->ProcessInput(dt);
        app->OnUpdate(dt);

        glClearColor(0, 0.4, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        //        Breakout->Render();


        glfwSwapBuffers(window);
    }

    ResourceManager::Clear();

    glfwTerminate();

    return 0;
}

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    if (type == GL_DEBUG_TYPE_ERROR) {
        LOG_PURE_ERROR("{} type = 0x{:x} | severity = 0x{:x} \n\t{}", "[GL]",
                       type, severity, message);
    }
    else {
        LOG_DEBUG("{} type = 0x{:x} | severity = 0x{:x} \n\t{}", "[GL]",
                  type, severity, message);
    }
}

void framebuffer_resize_cb(GLFWwindow *wndow, int w, int h)
{
    glViewport(0, 0, w, h);
}

void key_cb(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key > 0 && key <= 1024) {
        if (action == GLFW_PRESS) {
            //            Breakout->m_keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            //            Breakout->m_keys[key]         = false;
            //            Breakout->m_KeyProcessed[key] = false;
        }
    }
}


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

    GLFWwindow *window = glfwCreateWindow(WIN_W, WIN_H, "GL", nullptr, nullptr);

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

    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *version     = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    GLint          major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);


    // During init, enable debug output
    // Notice: this is a specific driver extension
    if (glDebugMessageCallback != nullptr) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, nullptr);
        LOG_DEBUG("Bound GL debug callback successfully");
    }
    else {
        LOG_WARN("glDebugMessageCallback is nullptr. Maybe your driver is not supportting this extionsion!");
    }

    return window;
}
