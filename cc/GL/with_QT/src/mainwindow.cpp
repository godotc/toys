#include "qlocale.h"
#include "qmenu.h"
#include "qobjectdefs.h"
#include "qwidget.h"
#include "ui_mainwindow.h"
#include <qlayout.h>

#include "QtGlWidget.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      GLW(new QtGlWidget(this)),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(GLW);

    this->resize(800, 600);
    this->TestMenu = new QMenu("Test", this);
    ui->menuBar->addMenu(TestMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}
