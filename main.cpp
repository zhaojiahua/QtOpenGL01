#include "QtOpenGL01.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtOpenGL01 w;
    w.show();
    return a.exec();
}
