#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtGlWidget.h"
#include "qaction.h"
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
    void initWireframeAction();

  private:
    Ui::MainWindow *ui;

    QtGlWidget *GLW;
    QMenu      *TestMenu;

    QAction *actionWireframe;
};

#endif // MAINWINDOW_H
