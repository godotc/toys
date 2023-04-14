#include "QtGlWidget.h"
#include "qopenglext.h"
#include "qopenglwidget.h"
#include <GL/gl.h>
#include <qdebug.h>
#include <string>

// Create VAO, VBO object, get uinque ID
static GLuint VAO, VBO, EBO;
static GLuint vertShader, fragShader;
static GLuint program;

static const char *vertSource = R"R(
#version 330 core
layout(location =0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos,1);
}
)R";
static const char *fragSource = R"R(
#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)R";

QtGlWidget::QtGlWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void QtGlWidget::initializeGL()
{


    initializeOpenGLFunctions();

    float vertices[] = {
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        -0.5, +0.5, 0.0,
        +0.5, +0.5, 0.0};

    GLuint indices[] = {
        0, 1, 3,
        0, 3, 2};

    {
        // VAO is symbol, VBO is its data
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        {
            // Bind VAO,VBO to current state
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                // NOTICE: attribute is ref to GL_ARRAY_BUFFER, must VAO->VBO->Attrib-> unbind
                // It's a state machine!
                // VAP index, stride, element type, normalized, size, first offset
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
            }
            glDisableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        // only  outline
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    {
        GLint success;
        char  log[512];

        vertShader = glCreateShader(GL_VERTEX_SHADER);
        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        program    = glCreateProgram();

        glShaderSource(vertShader, 1, &vertSource, nullptr);
        glCompileShader(vertShader);
        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE) {
            glGetShaderInfoLog(vertShader, 512, nullptr, log);
            qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << log;
        }

        glShaderSource(fragShader, 1, &fragSource, nullptr);
        glCompileShader(fragShader);
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE) {
            glGetShaderInfoLog(fragShader, 512, nullptr, log);
            qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << log;
        }

        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (success != GL_TRUE) {
            glGetProgramInfoLog(program, 512, nullptr, log);
            qDebug() << "ERROR::PROGRAM::LINKERROR: " << log;
        }


        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }
}

void QtGlWidget::resizeGL(int w, int h) {}

void QtGlWidget::paintGL()
{
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //  glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
