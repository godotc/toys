
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
attribute vec3 inColor;
varying vec3 outColor;

attribute vec2 inUvs;
varying vec2 outUvs;

uniform mat4 matrix;
uniform float time;

void main()
{
    float theta = time;

    float co = cos(theta);
    float si = sin(theta);

    mat4 rotationY = mat4(
        co, 0, si,0,
        0 , 1, 0, 0,
        -si, 0, co, 0,
        0, 0 , 0,  1 );


    mat4 rotationX = mat4(
        1, 0, 0,  0,
        0, co, -si,0,
        0 ,si, co, 0,
        0, 0 , 0, 1 );

    outUvs = inUvs;
    outColor = inColor;
    gl_Position =  matrix *rotationX * rotationY * vec4(inPosition, 1.f);
}
)END";

const GLchar *raster120 = R"END(
#version 120
varying vec3 outColor;
varying vec2 outUvs;
uniform sampler2D tex; // 1st texture slot by default

void main()
{
    gl_FragColor = vec4(outColor, 1.f)/2.f + vec4 ( texture2D( tex, outUvs)) /2.f;
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


    GLfloat vertices[] =
        {-1, -1, +1, // 0
         -1, +1, +1,
         +1, +1, +1,
         +1, -1, +1,

         -1, -1, -1, // 4
         -1, +1, -1,
         +1, +1, -1,
         +1, -1, -1,

         -1, -1, -1, // 8
         -1, +1, -1,
         +1, +1, -1,
         +1, -1, -1};

    GLfloat colors[] =
        {0, 0, 1, // 0
         0, 1, 0,
         1, 0, 0,
         1, 0, 1,

         0, 1, 0, // 4
         0, 1, 0,
         1, 1, 0,
         1, 0, 0,

         0, 1, 0, // additional
         0, 1, 0,
         1, 1, 0,
         1, 0, 0};

    GLubyte indices[] = {
        0, 1, 2, 0, 2, 3,     // front
        0, 4, 5, 0, 5, 1,     // left
        1, 5, 6, 1, 6, 2,     // top
        0, 4, 7, 0, 7, 3,     // bot
        4, 7, 6, 4, 6, 5,     // back
        7, 3, 2, 7, 2, 6,     // right
        8, 9, 10, 8, 10, 11}; // a ddition

    GLuint verticesBuf;
    glGenBuffers(1, &verticesBuf);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint colorBuf;
    glGenBuffers(1, &colorBuf);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint indicesBuf;
    glGenBuffers(1, &indicesBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



    // -------------------------------TEXTURE

    stbi_uc *bitmap = nullptr;
    int      x = 0, y = 0;
    int      channel = 0;

    bitmap = stbi_load("res/brick.bmp", &x, &y, &channel, 0);
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

    // --------------------- text attrib
    GLfloat uvs[] =
        {0, 0,
         0, 0,
         0, 0,
         0, 0,
         0, 0,
         0, 0,
         0, 0,
         0, 0,
         0, 0, // full rect for out additional "overlay" side
         0, 1,
         1, 1,
         1, 0};

    GLuint uvsData;
    glGenBuffers(1, &uvsData);
    glBindBuffer(GL_ARRAY_BUFFER, uvsData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    GLuint attribUvs;
    attribUvs = glGetAttribLocation(shaderProgram, "inUvs");
    glEnableVertexAttribArray(attribUvs);
    glBindBuffer(GL_ARRAY_BUFFER, uvsData);
    glVertexAttribPointer(attribUvs, 2, GL_FLOAT, GL_FALSE, 0, 0);



    // ------------------------- Attrib
    GLfloat matrix[] =
        {-0.5, 0, 0, 0, // 翻转 x 与 y轴
         0, -0.5, 0, 0,
         0, 0, 0.5, 0,
         0, 0, 0, 1};

    GLuint unifromMatrix;
    unifromMatrix = glGetUniformLocation(shaderProgram, "matrix");
    glUniformMatrix4fv(unifromMatrix, 1, GL_FALSE, matrix);


    GLuint attribPosition;
    attribPosition = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(attribPosition);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuf);
    glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint attribColor;
    attribColor = glGetAttribLocation(shaderProgram, "inColor");
    glEnableVertexAttribArray(attribColor);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuf);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, 0);


    GLuint uniformTime;
    uniformTime = glGetUniformLocation(shaderProgram, "time");

    // glEnable(GL_CULL_FACE); // 隐藏所有背面的三角形


    // 4. render loop
    while (!glfwWindowShouldClose(p_window))
    {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        GLfloat time = glfwGetTime();
        glUniform1f(uniformTime, time);


        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    glfwTerminate();

    cout << "program closed" << endl;
    return 0;
}
