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
#include <iostream>
#include <memory>
#include <stdexcept>

#include "render/camera.h"

#include "gl_macros.h"

#include <glm/common.hpp>
#include <glm/glm.hpp>
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
    }

    ~App() {}

    void Init()
    {
        init_cube_vertex();

        ResourceManager::LoadShader(FPath("res/shader/cube.glsl"), "cube");

        ResourceManager::LoadTexture(FPath("res/texture/arch.png"), "arch");
        glActiveTexture(GL_TEXTURE0);

        glm::mat4 model_mat(1.f);
        //        model_mat= glm::scale(glm::mat4(1.f), {0.5, 0.5, 0.5});                  // world scale
        model_mat = glm::rotate(model_mat, glm::radians(-55.f), {1.f, 0.f, 0.f}); // world rotation
        glm::mat4 view_mat(1.f);
        view_mat = glm::translate(view_mat, {0, 0, -3.f});
        //        viewmodel_mat = glm::scale(glm::mat4(1.0), {100, 100, 100});
        viewmodel_mat = view_mat * model_mat;

        project_mat = glm::perspective(glm::radians(45.f), WIN_H / (float)WIN_H, 0.1f, 100.f);
    }

    void OnRenderer()
    {
        auto Texture = ResourceManager::GetTexture("arch");

        Texture.Bind();
        ResourceManager::GetShader("cube")
            .Use()
            .SetInteger("u_Texure", Texture.ID)
            .SetMatrix4("viewmodel", viewmodel_mat)
            .SetMatrix4("projection", project_mat)
            .SetVector4f("color", {1, 1, 1, 1});


        glPointSize(20.f);
        glBegin(GL_POINTS);
        glVertex3f(0, 1, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(-1, 0, 0);
        glVertex3f(0, -1, 0);
        glEnd();


        glBindVertexArray(cube_va);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }
    void OnUpdate(float dt)
    {
    }

    void ProcessInput(float dt)
    {
    }

    void init_cube_vertex()
    {
        GLfloat vertices[] = {
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // 0
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // 1
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // 2
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // 3

            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 4
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 5
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 6
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f   // 7
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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));

            GL_CALL(glGenBuffers(1, &cube_ib));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ib);
            GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glBindVertexArray(0);
        GL_CHECK_HEALTH();


        GL_CHECK_HEALTH();
    }

    Camera camera = {
        {0, 0, 3},
        {0, 0, 0},
        {0, 1, 0}
    };

    GLuint cube_va, cube_ib;

    glm::mat4 project_mat{1.f};
    glm::mat4 viewmodel_mat{1.f};
};



int main(int argc, char **argv)
{
    __logcpp::SetLogLevel(__logcpp::LogLevel::L_DEBUG);
    GLFWwindow *window = init_glfw();
    LOG_LOG("GLFW has initialize successfully");


    // OpenGL configuration
    glViewport(0, 0, WIN_W, WIN_H);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    App *app = new App();
    app->Init();

    float dt         = 0.f;
    float last_frame = 0.f;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2, 0.4, 0.3, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_frame = glfwGetTime();
        dt                   = current_frame - last_frame;
        last_frame           = current_frame;
        // LOG_DEBUG("dt: {}", dt);

        app->OnRenderer();
        app->ProcessInput(dt);
        app->OnUpdate(dt);


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
    //    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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
