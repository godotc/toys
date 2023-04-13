#include "QtGlWidget.h"
#include "qopenglext.h"
#include "qopenglwidget.h"
#include <GL/gl.h>
#include <qdebug.h>

// Create VAO, VBO object, get uinque ID
static GLuint VAO, VBO;
static GLuint vertShader, fragShader;
static GLuint program;

static const char *vertSource = R"R(
#version 330 core
layout(location =0) in vec3 aPos;
void main(){
	gl_Position = vec4(aPos,1);
}
)R";
static const char *fragSource = R"R(
#version 330 core
out vec4 FragColor;
void main(){
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)R";

QtGlWidget::QtGlWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void QtGlWidget::initializeGL()
{
    initializeOpenGLFunctions();

    {
        float vertices[] = {
            -0.5,
            -0.5,
            0.0,

            0.5,
            -0.5,
            0.0,

            0.0,
            +0.5,
            0.0,
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Bind VAO,VBO to current state
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Create a buffer in graphic device
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // VAP index, stride, element type, normalized, size, first offset
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
                              (void *)0);

        glEnableVertexAttribArray(0);

        // Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    {
        GLint success;
        char  log[512];

        glShaderSource(vertShader, 1, &vertSource, nullptr);
        glCompileShader(vertShader);
        if (!success) {
            glGetShaderInfoLog(vertShader, 512, nullptr, log);
            qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << log;
        }

        glShaderSource(fragShader, 1, &fragSource, nullptr);
        glCompileShader(fragShader);
        if (!success) {
            glGetShaderInfoLog(fragShader, 512, nullptr, log);
            qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << log;
        }

        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        // TODO: finsh program , use , unuse, delte....
        if (!)
    }
}

void QtGlWidget::resizeGL(int w, int h) {}

void QtGlWidget::paintGL()
{
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
