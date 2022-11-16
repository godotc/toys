
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <sys/stat.h>

#include <math.h>
#include <stdexcept>

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>



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

    const char *filenaem = "res/brick.bmp";
    struct stat buf;
    ::stat(filenaem, &buf);
    cout << "file size:" << buf.st_size << endl;


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

    stbi_uc *bitmap = nullptr;
    int      x = 0, y = 0;
    int      channel = 0;

    bitmap = stbi_load(filenaem, &x, &y, &channel, 0);
    if (bitmap == nullptr) {
        throw std::runtime_error("Texture load error");
    }
    else {
        cout << "x:" << y << endl;
        cout << "y:" << y << endl;
        cout << "channel:" << channel << endl;
    }

    GLuint texId;
    glGenTextures(1, &texId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, (void *)bitmap);



    // ------------------------- Attrib
    GLuint unifromMatrix;
    unifromMatrix = glGetUniformLocation(shaderProgram, "matrix");
    glUniformMatrix4fv(unifromMatrix, 1, GL_FALSE, matrix);


    GLuint attribPos;
    attribPos = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(attribPos);
    glBindBuffer(GL_ARRAY_BUFFER, positionData);
    glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE, 0, 0);


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

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    glfwTerminate();

    cout << "program closed" << endl;
    return 0;
}
