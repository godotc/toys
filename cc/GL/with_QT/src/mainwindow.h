#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtGlWidget.h"
#include "qobjectdefs.h"
#include <QMainWindow>

class QtGlWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


  private:
    Ui::MainWindow *ui;

    QtGlWidget *GLW;
    QMenu      *TestMenu;
};

#endif // MAINWINDOW_H
