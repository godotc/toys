
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <math.h>
#include <stdexcept>

using std::cout, std::endl;

const GLfloat vertices[] = {
    1.0f, 1.0f, 0.0f, //×, y，Z
    1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f};

const GLfloat colors[] = {
    0.0f, 0.0f, 1.0f, // r，g, b
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f};

const GLchar *vertex120 = R"END(
#version 120
attribute vec4 inColor;
attribute vec4 inPosition;
uniform mat4 matrix;
varying vec4 outColor;

void main()
{
	outColor = inColor;
	gl_Position = inPosition * matrix; // 
}
)END";

const GLchar *raster120 = R"END(
#version 120
varying vec4 outColor;
void main()
{
	gl_FragColor = outColor;
}
)END";


int main()
{
    std::cout << "hello world" << std::endl;

    GLFWwindow *p_window;

    // 1. init
    if (!glfwInit()) {
        throw std::runtime_error("glfw init error!");
    }

    const GLubyte *bytes = glGetString(GL_VERSION);
    std::cout << "GL verison: " << bytes << endl;

    // 2. create window
    p_window = glfwCreateWindow(800, 800, "Hello GLFW", 0, 0);
    if (!p_window) {
        throw std::runtime_error("create window error");
    }

    // 3. bind window context
    glfwMakeContextCurrent(p_window);


    GLint compilationStatus;

    // vertex/fragment shader
    GLuint vertexShader;
    {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertex120, 0);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationStatus);
        if (compilationStatus == GL_FALSE) {
            GLchar message[256];
            glGetShaderInfoLog(vertexShader, sizeof(message), 0, message);
            cout << "erro when compiling vertex-shader: " << message << endl;
            exit(-1);
        }
    }
    GLuint fragmentShader;
    {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &raster120, 0);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);
        if (compilationStatus == GL_FALSE) {
            GLchar message[256];
            glGetShaderInfoLog(vertexShader, sizeof(message), 0, &message[0]);
            cout << "erro when compiling-fragmentShader: " << message << endl;
            exit(-1);
        }
    }


    // shader program
    GLuint shaderProgram;
    {
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetShaderiv(shaderProgram, GL_LINK_STATUS, &compilationStatus);
        if (compilationStatus == GL_FALSE) {
            GLchar message[256];
            glGetProgramInfoLog(shaderProgram, sizeof(message), 0, &message[0]);
            cout << "erro when linking program: " << message << endl;
            exit(-1);
        }
    }

    // aply program
    glUseProgram(shaderProgram);



    // VBO setup
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);

    // send data to GPU (memcpy)
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


    // Attribute
    GLuint attribPos;
    {
        attribPos = glGetAttribLocation(shaderProgram, "inPosition");
        glEnableVertexAttribArray(attribPos);
        // bind buffer to attrib
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    GLuint attribColor;
    {
        attribColor = glGetAttribLocation(shaderProgram, "inColor");
        glEnableVertexAttribArray(attribColor);
        // bind buffer to attrib
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }



    // uniform
    GLuint unifromMatrix = glGetUniformLocation(shaderProgram, "matrix");

    float alpha = 0;

    // 4. render loop
    while (!glfwWindowShouldClose(p_window))
    {

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        float sa = 0.5 * sin(alpha);
        float ca = 0.5 * cos(alpha);
        alpha += 0.1;

        const GLfloat matrix[] = {
            sa, -ca, 0, 0,
            ca, sa, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};

        glUniformMatrix4fv(unifromMatrix, 1, GL_FALSE, matrix);


        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    glfwTerminate();

    cout << "program closed" << endl;
    return 0;
}
