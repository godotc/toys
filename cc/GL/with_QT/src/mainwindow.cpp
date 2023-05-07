#include "qaction.h"
#include "qdebug.h"
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
      ui(new Ui::MainWindow),
      GLW(new QtGlWidget(this))
{
    ui->setupUi(this);
    setCentralWidget(GLW);

    this->resize(800, 600);
    this->TestMenu = new QMenu("Test", this);
    ui->menuBar->addMenu(TestMenu);


    this->initWireframeAction();

    connect(ui->actionRect, &QAction::triggered, this, [this]() {
        qDebug() << "Action draw rect";
        GLW->DrawShape(Shape::Rect);
    });
    connect(ui->actionClear, &QAction::triggered, this, [this]() {
        qDebug() << "Action clear";
        GLW->DrawShape(Shape::None);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initWireframeAction()
{
    this->actionWireframe = new QAction("Wireframe", this);
    actionWireframe->setCheckable(true);

    // NOTICE: Need configure the action style in context(parent widget)'s styleSheet'
    // if (!actionWireframe->setProperty("styleSheet", "QWidget::checked {background-color:rgb(150,150,150);}")) {
    //    qDebug() << "Failed to set the actionWireframe's styleSheet";
    // }
    // use this->setStypeSheet(), but I have configure some other style before...

    ui->mainToolBar->addAction(actionWireframe);

    auto ctx = __FUNCTION__;

    connect(actionWireframe, &QAction::triggered, this,
            [this, ctx]() {
                const auto newState = actionWireframe->isChecked();
                qDebug() << ctx << ": Toggle Wireframe mode to [" << (newState ? "Line" : "Fill") << "]";
                GLW->SetWireFrameMode(newState);
            });
}
