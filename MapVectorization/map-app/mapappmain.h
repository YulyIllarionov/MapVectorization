#ifndef MAPAPPMAIN_H
#define MAPAPPMAIN_H

#include <QtWidgets/QMainWindow>
#include "ui_mapappmain.h"

class mapAppMain : public QMainWindow
{
    Q_OBJECT

public:
    mapAppMain(QWidget *parent = 0);
    ~mapAppMain();

private:
    Ui::mapAppMainClass ui;
};

#endif // MAPAPPMAIN_H
