#pragma once

#include "qobjectdefs.h"
#include <qopenglfunctions_3_3_core.h>
#include <qopenglwidget.h>

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

  signals:
  private:
};
