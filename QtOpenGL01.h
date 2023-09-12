#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_QtOpenGL01.h"

class QtOpenGL01 : public QMainWindow
{
    Q_OBJECT

public:
    QtOpenGL01(QWidget *parent = nullptr);
    ~QtOpenGL01();

private:
    Ui::QtOpenGL01Class ui;
    
    void ActionDrawRectTriggered();
    void ActionClearAllTriggered();
    void ActionWireFarmeTriggered();
    void actionDrawIcosahedronTriggered();
};
