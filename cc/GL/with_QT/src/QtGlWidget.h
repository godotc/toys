#pragma once

#include "qmap.h"
#include "qobjectdefs.h"
#include "qopenglshaderprogram.h"
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <qopenglfunctions_3_3_core.h>
#include <qopengltexture.h>
#include <qopenglwidget.h>


enum Shape
{
    None,
    Rect,
    Circle,
    Triangle
};

class QtGlWidget
    : public QOpenGLWidget,
      public QOpenGLFunctions_3_3_Core
{
  public:
    explicit QtGlWidget(QWidget *parent);
    ~QtGlWidget();

    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

  public:
    void DrawShape(Shape shape);

  signals:
    void OnTimeout();

  public:
    void SetWireFrameMode(bool bWireframe);

  private:
    Shape                m_Shape;
    QOpenGLShaderProgram m_ShaderProgram;
    QTimer               m_Timer;

    QString                         m_Texture;
    QMap<QString, QOpenGLTexture *> m_Textures;
};
