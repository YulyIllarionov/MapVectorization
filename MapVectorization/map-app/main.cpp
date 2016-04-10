#include "stdafx.h"
#include "mapappmain.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mapAppMain w;
    w.show();
    return a.exec();
}
