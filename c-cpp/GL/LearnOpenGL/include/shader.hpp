#ifndef GSTL_SHADER_H
#define GSTL_SHADER_H

#include <cstdio>
#include <fstream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iterator>
#include <math.h>

#include <cstdlib>
#include <dbstl_exception.h>
#include <memory>
#include <sstream>
#include <string>

#include <functional>
#include <sys/types.h>

using std::cout;
using std::endl;
using std::string;
using t_UI = unsigned int;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);



class Shader
{

  public:

    //! shader ID
    t_UI ID;

    Shader(string &vertexShaderPath, string &fragmentShaderPath);
    Shader(string &intergrateFile); // 当 vert 与 frag 写在同一个文件

    void Use();

    // uniform 工具函数
    template <typename T>
    void setUnifrom(const string &name, T value) const;

  private:
    void initProgram(string &vertSource, string &fragSource);
    t_UI getProgram(t_UI vert, t_UI frag);
    void testCompile(t_UI shaderId, std::string &errorPrefix);
    t_UI getShader(const char *source, GLenum shaderType,
                   std::string &&errorPrefix, GLint *place_holder);
};


Shader::Shader(string &vertexShaderPath, string &fragmentShaderPath)
{
    string vertSource, fragSource;

    std::ifstream vsFile, fsFile;

    vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vsFile.open(vertexShaderPath);
        fsFile.open(fragmentShaderPath);

        std::stringstream vsStream, fsStream;

        vsStream << vsFile.rdbuf();
        fsStream << fsFile.rdbuf();

        vsFile.close();
        fsFile.close();

        vertSource = vsStream.str();
        fragSource = fsStream.str();
    }
    catch (std::ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    initProgram(vertSource, fragSource);
}

Shader::Shader(string &intergrateFile)
{

    std::ifstream File;

    enum ShaderType
    {
        NONE   = -1,
        VERTEX = 0,
        FRAGMENT
    };
    std::stringstream stream[2];

    File.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        File.open(intergrateFile);

        ShaderType type = ShaderType::NONE;

        std::string line;
        while (std::getline(File, line))
        {
            if (line.find("#shader") != line.npos)
            {
                if (line.find("vertex") != line.npos)
                    type = VERTEX;
                else if (line.find("fragment") != line.npos)
                    type = FRAGMENT;
            }
            else {
                stream[(int)type] << line << "\n";
            }
        }

        File.close();
    }
    catch (std::ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    string &vertSource = stream[VERTEX].str();
    string &fragSource = stream[FRAGMENT].str();
    initProgram(stream[VERTEX].str(), stream[FRAGMENT].str());
}


void Shader::initProgram(string &vertSource, string &fragSource)
{
    auto vs = getShader(vertSource.c_str(), GL_VERTEX_SHADER, "ERROR::SHADER::VERTEX::COMPILATION_FAILURE", nullptr);
    auto fs = getShader(fragSource.c_str(), GL_FRAGMENT_SHADER, "ERROR::SHADER::VERTEX::COMPILATION_FAILURE", nullptr);
    ID      = getProgram(vs, fs);
}

const char *vertexShaderSource = R"S(
#version 330 core
in  vec3 aPos;
in  vec3 aColor;

out  vec3 outColor;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	outColor = aColor;
}
)S";

const char *fragmentShaderSource = R"S(
#version 330 core
out vec4 FragColor;

in vec3 outColor;

void main()
{
	FragColor = vec4(outColor, 1.);
}
)S";


void Shader::testCompile(t_UI shaderId, std::string &errorPrefix)
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

t_UI Shader::getShader(const char *source, GLenum shaderType, std::string &&errorPrefix, GLint *place_holder)
{
    t_UI shaderId;

    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &source, place_holder);
    glCompileShader(shaderId);
    testCompile(shaderId, errorPrefix);

    return shaderId;
}

t_UI Shader::getProgram(t_UI vert, t_UI frag)
{
    t_UI shaderProgram;

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
void bindAndEnData(t_UI bindBufferId, GLenum bufferType, size_t dataSize, void *data, uint drawType)
{
    glBindBuffer(bufferType, bindBufferId);
    glBufferData(bufferType, dataSize, data, drawType);
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

    int nAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
    cout << "Maxium nr of vertex attributes supported: " << nAttributes << endl;

    glViewport(0, 0, 800, 800);

    glfwMakeContextCurrent(p_window);
    glfwSetFramebufferSizeCallback(p_window, framebuffer_size_callback);


    //--------------------------- Shader & Program

    t_UI vertexShader;
    t_UI fragmentShader;
    t_UI shaderProgram;
    vertexShader   = getShader(vertexShaderSource, GL_VERTEX_SHADER, "ERROR::SHADER::VERTEX::COMPILATION_FAILED", nullptr);
    fragmentShader = getShader(fragmentShaderSource, GL_FRAGMENT_SHADER, "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED", nullptr);
    shaderProgram  = getProgram(vertexShader, fragmentShader);


    //----------------------------- VBO, VAO, EBO

    float vertices[] =
        {
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
        };

    unsigned int indices[] =
        {
            // 注意索引从0开始!
            // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
            // 这样可以由下标代表顶点组合成矩形
            0, 1, 2, // 第一个三角形
                     // 0, 2, 3  // 第二个三角形
        };

    t_UI VAO;
    t_UI VBO;
    t_UI EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the vertex arr obj first, then bind vertex buf,
    // and then confiure vertex attribue
    glBindVertexArray(VAO);

    bindAndEnData(VBO, GL_ARRAY_BUFFER,
                  sizeof(vertices), vertices, GL_STATIC_DRAW);

    bindAndEnData(EBO, GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



    //--------------------------- Attribute
    //! 位置(location=0)， 顶点大小， 类型， normalization，步长（为0自动判断或自己指定），位置数据在缓存中的offset(起始位置)
    int attribPos = glGetAttribLocation(shaderProgram, "aPos");
    glEnableVertexAttribArray(attribPos);
    glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *)0);


    int attribColor = glGetAttribLocation(shaderProgram, "aColor");
    glEnableVertexAttribArray(attribColor);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *)(3 * sizeof(float))); // 颜色offset为3步长为6


    // Unbind the buffer and array
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //-------------------------- Main Loop
    while (!glfwWindowShouldClose(p_window))
    {
        processInput(p_window);

        glClearColor(0.0, 0.3, 0.3, 0.8);
        glClear(GL_COLOR_BUFFER_BIT);


        // bind target
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);


    glfwTerminate();
    return 0;
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

#endif
