#ifndef GSTL_SHADER_H
#define GSTL_SHADER_H

#include <array>
#include <cstdio>
#include <fstream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iterator>

#include <cstdlib>
#include <dbstl_exception.h>
#include <memory>
#include <sstream>
#include <string>


#include <functional>
#include <sys/types.h>
#include <type_traits>

using std::cout;
using std::endl;
using std::string;
using T_UI = unsigned int;


class Shader
{

  public:
    //! shader ID

    Shader(string &vertexShaderPath, string &fragmentShaderPath);
    Shader(string &theIntengrateFile); // 当 vert 与 frag 写在同一个文件
    Shader(const char *vertexStr, const char *fragmentStr);

    Shader(const Shader &)            = delete;
    Shader &operator=(const Shader &) = delete;

    Shader(Shader &&other) noexcept            = default;
    Shader &operator=(Shader &&other) noexcept = default;

    ~Shader();


    void Use() const;
    void UnUse() const;

    // uniform 工具函数
    template <typename T>
    void setUnifrom(const string &name, T value) const;

    [[nodiscard("Shader program ID")]] constexpr T_UI getID() const noexcept { return ID; };

  protected:
    T_UI ID;

  private:
    void        initProgram(string &vertSource, string &fragSource);
    void        initProgram(const char *vertSource, const char *fragSource);
    static T_UI getProgram(T_UI vert, T_UI frag);
    static void testCompile(T_UI shaderId, std::string &errorPrefix);
    static T_UI getShader(const char *source, GLenum shaderType, std::string &&errorPrefix, GLint *place_holder);
};


Shader::Shader(string &vertexShaderPath, string &fragmentShaderPath)
{
    string vertSource;
    string fragSource;

    std::ifstream vsFile;
    std::ifstream fsFile;

    vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vsFile.open(vertexShaderPath);
        fsFile.open(fragmentShaderPath);

        std::stringstream vsStream;
        std::stringstream fsStream;

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

Shader::Shader(string &theIntengrateFile)
{
    std::ifstream file;

    enum ShaderType
    {
        NONE   = -1,
        VERTEX = 0,
        FRAGMENT
    };
    std::array<std::stringstream, 2> stream;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(theIntengrateFile);

        ShaderType type = ShaderType::NONE;

        std::string line;
        while (std::getline(file, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos) {
                    type = VERTEX;
                }
                else if (line.find("fragment") != std::string::npos) {
                    type = FRAGMENT;
                }
            }
            else {
                stream[(int)type] << line << "\n";
            }
        }

        file.close();
    }
    catch (std::ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    initProgram(stream[VERTEX].str().c_str(), stream[FRAGMENT].str().c_str());
}

Shader::Shader(const char *vertexStr, const char *fragmentStr)
{
    initProgram(vertexStr, fragmentStr);
}

void Shader::initProgram(string &vertSource, string &fragSource)
{
    initProgram(vertSource.c_str(), fragSource.c_str());
}
void Shader::initProgram(const char *vertSource, const char *fragSource)
{
    auto vertShader = getShader(vertSource, GL_VERTEX_SHADER, "ERROR::SHADER::VERTEX::COMPILATION_FAILURE", nullptr);
    auto fragShader = getShader(fragSource, GL_FRAGMENT_SHADER, "ERROR::SHADER::VERTEX::COMPILATION_FAILURE", nullptr);
    ID              = getProgram(vertShader, fragShader);
}


void Shader::testCompile(T_UI shaderId, std::string &errorPrefix)
{
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(shaderId, sizeof(infoLog),
                           nullptr, infoLog);
        std::cout << errorPrefix << "\n"
                  << infoLog << std::endl;
        exit(-1);
    }
}

T_UI Shader::getShader(const char *source, GLenum shaderType, std::string &&errorPrefix, GLint *place_holder)
{
    T_UI shaderId;

    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &source, place_holder);
    glCompileShader(shaderId);
    testCompile(shaderId, errorPrefix);

    return shaderId;
}

T_UI Shader::getProgram(T_UI vert, T_UI frag)
{
    T_UI shaderProgram;

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
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

void Shader::Use() const
{
    glUseProgram(ID);
}
void Shader::UnUse() const
{
    glUseProgram(-1);
}

template <typename T>
void Shader::setUnifrom(const string &name, T value) const
{
    if (std::is_same<bool, T>::value)
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    else if (std::is_same<int, T>::value)
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    else if (std::is_same<float, T>::value)
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
}

#endif
