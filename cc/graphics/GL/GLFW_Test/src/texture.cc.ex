
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <math.h>
#include <stdexcept>

using std::cout, std::endl;

const GLchar *vertex120 = R"END(
#version 120
attribute vec3 inPosition;
attribute vec2 inUvs;
varying vec2 outUvs;
uniform mat4 matrix;

void main()
{
    outUvs = inUvs;
    gl_Position = matrix * vec4(inPosition, 1);
}
)END";

const GLchar *raster120 = R"END(
#version 120
uniform vec2 res; // resolution
uniform float  time;
varying vec2 outUvs;
uniform sampler2D tex; // 1st texture slot by default

void main()
{
    gl_FragColor = texture2D(tex,outUvs);
}
)END";

int main()
{
    std::cout << "hello world" << std::endl;



    // --------------------------- init
    GLFWwindow *p_window;

    if (!glfwInit()) {
        throw std::runtime_error("glfw init error!");
    }

    const GLubyte *bytes = glGetString(GL_VERSION);
    std::cout << "GL verison: " << bytes << endl;

    p_window = glfwCreateWindow(800, 800, "Hello GLFW", 0, 0);
    if (!p_window) {
        throw std::runtime_error("create window error");
    }

    glfwMakeContextCurrent(p_window);

    // -------------------------------------- shader and program

    GLuint vertexShader;
    GLint  compilationStatus;
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



    // --------------------------------------------VBOs;


    GLfloat matrix[] =
        {-1, 0, 0, 0, // 翻转 x 与 y轴
         0, -1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1};

    GLfloat positions[] =
        {-1, -1, 0,
         -1, 1, 0,
         1, -1, 0,
         1, -1, 0,
         -1, 1, 0,
         1, 1, 0};

    GLuint positionData;
    glGenBuffers(1, &positionData);
    glBindBuffer(GL_ARRAY_BUFFER, positionData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); // memcpy() to GPU's buf


    GLfloat uvs[] =
        {0, 0,
         0, 1,
         1, 0,
         1, 0,
         0, 1,
         1, 1};

    GLuint uvsData;
    glGenBuffers(1, &uvsData);
    glBindBuffer(GL_ARRAY_BUFFER, uvsData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);


    // -------------------------------TEXTURE
#pragma pack(push)
#pragma pack(1)
    struct Pixel
    {
        GLubyte r;
        GLubyte g;
        GLubyte b;
    };
#pragma pack(pop)

    Pixel r = {0xFF, 0x00, 0x00};
    Pixel g = {0x00, 0xFF, 0x00};
    Pixel b = {0x00, 0x00, 0xFF};
    Pixel y = {0xFF, 0xFF, 0x00};
    Pixel d = {0x33, 0x33, 0x33};
    Pixel m = {0xFF, 0x00, 0xFF};
    Pixel o = {0x00, 0x00, 0x00};


    Pixel texture[] =
        {y, y, y, y, y,
         y, o, o, o, y,
         o, r, o, r, o,
         o, o, o, o, o,
         o, o, g, o, o,
         o, b, b, b, o};

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 5, 5, 0, GL_RGB, GL_UNSIGNED_BYTE, (void *)texture);



    // ------------------------- Attrib
    GLuint unifromMatrix;
    unifromMatrix = glGetUniformLocation(shaderProgram, "matrix");
    glUniformMatrix4fv(unifromMatrix, 1, GL_FALSE, matrix);


    GLuint attribPos;
    attribPos = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(attribPos);
    glBindBuffer(GL_ARRAY_BUFFER, positionData);
    glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint uniformRes = glGetUniformLocation(shaderProgram, "res");
    glUniform2f(uniformRes, 600., 600.);

    GLuint uniformTime = glGetUniformLocation(shaderProgram, "time");


    //! tesxtre
    GLuint attriUvs;
    attriUvs = glGetAttribLocation(shaderProgram, "inUvs");
    glEnableVertexAttribArray(attriUvs);
    glBindBuffer(GL_ARRAY_BUFFER, uvsData);
    glVertexAttribPointer(attriUvs, 2, GL_FLOAT, GL_FALSE, 0, 0);



    // 4. render loop
    while (!glfwWindowShouldClose(p_window))
    {

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        glUniform1f(uniformTime, time);


        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    glfwTerminate();

    cout << "program closed" << endl;
    return 0;
}
