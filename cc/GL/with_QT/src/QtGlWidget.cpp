#include "QtGlWidget.h"
#include "qimage.h"
#include "qopenglext.h"
#include "qopenglshaderprogram.h"
#include "qopengltexture.h"
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
#include <type_traits>
#include <vector>

#include <stb/stb_image.h>

// Create VAO, VBO object, get uinque ID
static GLuint VAO, VBO, EBO;



QtGlWidget::QtGlWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    // m_Timer.start(41);
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

    std::vector<float>  vertices;
    std::vector<GLuint> indices;

    {
        indices = {
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
    }

    // textures sample
    {
        // 3 vertex 3 color  2 texture
        vertices = {
            0.5, 0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0,
            0.5, -0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0,
            -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
            -0.5, 0.5, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0};

        indices = {
            0, 1, 2,
            2, 3, 0};
    }



    { // VAO is symbol, VBO is its data
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        {
            // Bind VBO to VAO
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            {

                auto getInitalPtr = [](auto cnt, auto type) -> void * {
                    return (void *)((sizeof(decltype(type)) * cnt));
                };

                // NOTICE: attribute is ref to GL_ARRAY_BUFFER, must VAO->VBO->Attrib-> unbind
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, getInitalPtr(3, float()));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, getInitalPtr(6, float()));
                glEnableVertexAttribArray(2);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }


        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Draw as only outline
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // shader
    {
        bool success;
        m_ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/def.vert");
        m_ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/def.frag");
        success = m_ShaderProgram.link();
        if (!success) {
            qDebug() << "ERROR::Shaer" << m_ShaderProgram.log();
        }
    }

    // texture
    {
        glBindVertexArray(VAO);

        // GLuint texture;
        // glGenTextures(1, &texture);
        // glBindTexture(GL_TEXTURE_2D, texture);
        // int  w, h, nrChannel;
        // auto data = stbi_load("abc", &w, &h, &nrChannel, 0);


        m_Texture = "brick";
        m_Textures.insert("arch", new QOpenGLTexture(QImage(":/res/textures/arch.png").mirrored()));
        m_Textures.insert("brick", new QOpenGLTexture(QImage(":/res/textures/brick.bmp").mirrored()));
        glBindVertexArray(0);
    }
}

void QtGlWidget::resizeGL(int w, int h) {}

void QtGlWidget::paintGL()
{
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    m_ShaderProgram.bind();

    glBindVertexArray(VAO);

    switch (m_Shape) {
    case Rect:
        m_Textures[m_Texture]->bind(0);
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
