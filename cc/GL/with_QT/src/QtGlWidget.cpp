#include "QtGlWidget.h"
#include "qopenglext.h"
#include "qopenglshaderprogram.h"
#include "qopenglwidget.h"
#include "qtimer.h"
#include "qwindowdefs.h"
#include <GL/gl.h>
#include <QTime>
#include <cmath>
#include <iterator>
#include <math.h>
#include <qdebug.h>
#include <string>
#include <vector>


// Create VAO, VBO object, get uinque ID
static GLuint VAO, VBO, EBO;



QtGlWidget::QtGlWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    m_Timer.start(41);
    connect(&m_Timer, &QTimer::timeout, this, &QtGlWidget::OnTimeout);
}

QtGlWidget::~QtGlWidget()
{
    makeCurrent();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    doneCurrent();
}
void QtGlWidget::OnTimeout()
{
    makeCurrent();
    static int tv = 1;
    ++tv;
    m_ShaderProgram.setUniformValue("outColor",
                                    sin(tv),
                                    0,
                                    0,
                                    tan(tv));
    update();
    doneCurrent();
}

void QtGlWidget::initializeGL()
{

    initializeOpenGLFunctions();

    float vertices[] = {
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        -0.5, +0.5, 0.0,
        +0.5, +0.5, 0.0};
    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3};



    float triangle01[] = {
        -0.9, -0.5, 0.0,
        0.0, -0.5, 0.0,
        -0.45, +0.5, 0.0};

    float triangle02[] = {
        0.0, -0.5, 0.0,
        0.9, -0.5, 0.0,
        +0.45, +0.5, 0.0};

    float *trangle_srcs[2] = {triangle01, triangle02};

    { // VAO is symbol, VBO is its data
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        {
            // Bind VBO to VAO
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof(trangle_srcs), vertices, GL_STATIC_DRAW);
                // NOTICE: attribute is ref to GL_ARRAY_BUFFER, must VAO->VBO->Attrib-> unbind
                // It's a state machine!
                // VAP index, stride, element type, normalized, size, first offset
                // index means the layout of vertex shaer's variable
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
                glEnableVertexAttribArray(2);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }


        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Draw as only outline
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    {
        bool success;
        m_ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/def.vert");
        m_ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/def.frag");
        success = m_ShaderProgram.link();
        if (!success) {
            qDebug() << "ERROR::Shaer" << m_ShaderProgram.log();
        }
    }
}

void QtGlWidget::resizeGL(int w, int h) {}

void QtGlWidget::paintGL()
{
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    m_ShaderProgram.bind();

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);


    switch (m_Shape) {
    case Rect:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    case Circle:
    case Triangle:
    case None:
        break;
    }
}

void QtGlWidget::SetWireFrameMode(bool bWireframe)
{
    makeCurrent();
    glPolygonMode(GL_FRONT_AND_BACK, bWireframe ? GL_LINE : GL_FILL);
    update();
    doneCurrent();
}

void QtGlWidget::DrawShape(Shape shape)
{
    m_Shape = shape;
    update();
}
