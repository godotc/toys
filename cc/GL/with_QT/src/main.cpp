#include "mainwindow.h"
#include "qglobal.h"
#include <QApplication>
#include <QOpenGLWidget>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(shader);

    QApplication app(argc, argv);
    MainWindow   w;
    w.show();

    return app.exec();
}
