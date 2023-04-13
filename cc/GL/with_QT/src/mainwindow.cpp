#include "mainwindow.h"
#include "qlocale.h"
#include "qmenu.h"
#include "qwidget.h"
#include "ui_mainwindow.h"
#include <qlayout.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ui->statusBar->setLocale(QLocale("Botttom"));
    // ui->mainToolBar->setLocale(QLocale("Botttom"));
    this->resize(800, 600);
    this->TestMenu = new QMenu("Test", this);
    ui->menuBar->addMenu(TestMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}
