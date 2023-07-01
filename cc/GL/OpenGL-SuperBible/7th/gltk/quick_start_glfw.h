#include "gl_macros.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <log.h>
#include <stdexcept>

namespace gltk {

namespace glfw {
static int         m_Width  = 0;
static int         m_Height = 0;
static GLFWwindow *m_Window = nullptr;
} // namespace glfw

inline void ShowGlfwError()
{
    const char *error;
    glfwGetError(&error);
    std::cerr << error << '\n';
}

void InitGL_GLFW(const std::string &name, int w, int h)
{
    // 1. init
    if (GLFW_FALSE == glfwInit()) {
        throw std::runtime_error("glfw init error!");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


    glfw::m_Width  = w;
    glfw::m_Height = h;

    // 2. create window
    glfw::m_Window = glfwCreateWindow(glfw::m_Width, glfw::m_Height, "Hello GLFW", nullptr, nullptr);
    if (!glfw::m_Window) {
        throw std::runtime_error("create window error");
    }

    // 3. bind window context
    glfwMakeContextCurrent(glfw::m_Window);

    // 4. load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to init glad");
    }

    const GLubyte *bytes = glGetString(GL_VERSION);
    std::cout << "GL verison: " << bytes << std::endl;
    LOG("hello");


    // 5. render loop

    glViewport(0, 0, glfw::m_Width, glfw::m_Height);
}
} // namespace gltk
