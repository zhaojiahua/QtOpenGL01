#include "QtOpenGL01.h"
#include "ZjhOpenGLWidget.h"

QtOpenGL01::QtOpenGL01(QWidget *parent)    : QMainWindow(parent)
{
    ui.setupUi(this);
    setCentralWidget(ui.zjhOpenGLWidget);
    connect(ui.actionDrawRect, &QAction::triggered, this, &QtOpenGL01::ActionDrawRectTriggered);
    connect(ui.actionClearAll, &QAction::triggered, this, &QtOpenGL01::ActionClearAllTriggered);
    connect(ui.actionWireFarme, &QAction::triggered, this, &QtOpenGL01::ActionWireFarmeTriggered);
    connect(ui.actionDrawIcosahedron, &QAction::triggered, this, &QtOpenGL01::actionDrawIcosahedronTriggered);
}

QtOpenGL01::~QtOpenGL01()
{}

void QtOpenGL01::ActionDrawRectTriggered()
{
    ui.zjhOpenGLWidget->DrawShape(ZjhOpenGLWidget::rectangle);
}

void QtOpenGL01::ActionWireFarmeTriggered()
{
    ui.zjhOpenGLWidget->SetWireFrame(ui.actionWireFarme->isChecked());
}

void QtOpenGL01::actionDrawIcosahedronTriggered()
{
    ui.zjhOpenGLWidget->DrawShape(ZjhOpenGLWidget::Icosahedron);
}

void QtOpenGL01::ActionClearAllTriggered()
{
    ui.zjhOpenGLWidget->DrawShape(ZjhOpenGLWidget::none);
}


