#include "mainwindow.h"
#include <QApplication>
#include <QOpenGLWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow   w;
    w.show();

    return app.exec();
}
