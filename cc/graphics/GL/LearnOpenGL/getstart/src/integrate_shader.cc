
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <dbstl_exception.h>
#include <memory>
#include <string>

#include <functional>
#include <shader.hpp>

using std::cout;
using std::endl;
using nUI = unsigned int;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

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



//! sample:
//! getBufferObject(VAO, GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW)
void bindAndEnData(T_UI bindBufferId, GLenum bufferType, size_t dataSize, void *data, uint drawType)
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


    GLFWwindow *pWindow = glfwCreateWindow(800, 800, "LearnOpenGL", nullptr, nullptr);
    if (pWindow == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(pWindow);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    int nAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
    cout << "Maxium nr of vertex attributes supported: " << nAttributes << endl;

    glViewport(0, 0, 800, 800);

    glfwMakeContextCurrent(pWindow);
    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);


    //--------------------------- Shader & Program


    Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.Use();

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

    T_UI VAO;
    T_UI VBO;
    T_UI EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the vertex arr obj first, then bind vertex buf,
    // and then confiure vertex attribue
    glBindVertexArray(VAO);
    bindAndEnData(VBO, GL_ARRAY_BUFFER,
                  sizeof(vertices), vertices, GL_STATIC_DRAW);

    bindAndEnData(EBO, GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

     float texCoords[] =
        {0., 0.,   // left bot
         1., 0.,   // right bot
         0.5, 1.}; // top mid

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);



    //--------------------------- Attribute
    //! 位置(location=0)， 顶点大小， 类型， normalization，步长（为0自动判断或自己指定），位置数据在缓存中的offset(起始位置)
    int attribPos = glGetAttribLocation(shader.getID(), "aPos");
    glEnableVertexAttribArray(attribPos);
    glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *)nullptr);



    int attribColor = glGetAttribLocation(shader.getID(), "aColor");
    glEnableVertexAttribArray(attribColor);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *)(3 * sizeof(float))); // 颜色offset为3步长为6


    // Unbind the buffer and array
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //-------------------------- Main Loop
    while (glfwWindowShouldClose(pWindow) == 0)
    {
        processInput(pWindow);

        glClearColor(0.0, 0.3, 0.3, 0.8);
        glClear(GL_COLOR_BUFFER_BIT);


        // bind target
        shader.Use();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


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
