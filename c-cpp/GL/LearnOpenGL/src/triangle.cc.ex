#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <dbstl_exception.h>
#include <stdexcept>
#include <string>
#include <sys/types.h>

#include <functional>

using std::cout;
using std::endl;
using tUI = unsigned int;

const char *vertexShaderSource   = R"S(
#version 330 core
layout (location=0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos, 1.0);
}
)S";
const char *fragmentShaderSource = R"S(
#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(0.5, 0.5, 0.0, 1.);
}
)S";


static void testCompile(tUI shaderId, std::string &errorPrefix)
{
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, sizeof(infoLog),
                           nullptr, infoLog);
        std::cout << errorPrefix << "\n"
                  << infoLog << std::endl;
        exit(-1);
    }
}


tUI getShader(const char *source, GLenum shaderType, std::string &&errorPrefix, GLint *place_holder)
{
    tUI shaderId;

    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &source, place_holder);
    glCompileShader(shaderId);
    testCompile(shaderId, errorPrefix);

    return shaderId;
}

tUI getProgram(tUI vert, tUI frag)
{
    tUI shaderProgram;

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char msg[512];
        glGetProgramInfoLog(shaderProgram, sizeof(msg), nullptr, msg);
        cout << "ERROR::SHADER::PROGRAM::LINK_ERROR\n"
             << msg << endl;
        exit(-1);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return shaderProgram;
}

//! sample:
//! getBufferObject(VAO, GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW)
void bindAndEnData(tUI bindBufferId, GLenum bufferType, size_t dataSize, float *data, uint drawType)
{
    glBindBuffer(bufferType, bindBufferId);
    glBufferData(bufferType, dataSize, data, drawType);
}



void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}



void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    printf("hello world\n");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE); // 透明


    GLFWwindow *p_window = glfwCreateWindow(800, 800, "LearnOpenGL", nullptr, nullptr);
    if (p_window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(p_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, 800, 800);

    glfwMakeContextCurrent(p_window);
    glfwSetFramebufferSizeCallback(p_window, framebuffer_size_callback);


    //----------------------------- VBO

    float vertices[] =
        {-0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f, 0.5f, 0.0f};

    tUI VBO;
    glGenBuffers(1, &VBO);
    bindAndEnData(VBO, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //--------------------------- Shader & Program

    tUI vertexShader;
    tUI fragmentShader;
    tUI shaderProgram;
    vertexShader   = getShader(vertexShaderSource, GL_VERTEX_SHADER, "ERROR::SHADER::VERTEX::COMPILATION_FAILED", nullptr);
    fragmentShader = getShader(fragmentShaderSource, GL_FRAGMENT_SHADER, "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED", nullptr);
    shaderProgram  = getProgram(vertexShader, fragmentShader);

    glUseProgram(shaderProgram);



    //-------------------------- VAO
    tUI VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // 这里 把 VBO 的顶点数组复制到缓冲中供OpenGl 使用
    bindAndEnData(VBO, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



    //--------------------------- Attribute
    //! 位置(location=0)， 顶点大小， 类型， normalization，步长（为0自动判断或自己指定），位置数据在缓存中的offset(起始位置)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);



    //-------------------------- Main Loop
    while (!glfwWindowShouldClose(p_window))
    {
        processInput(p_window);

        glClearColor(0.0, 0.3, 0.3, 0.8);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
