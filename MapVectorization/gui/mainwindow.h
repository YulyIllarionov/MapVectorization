#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include "base_types.h"
#include"imageviewer.h"
#include "layersviewer.h"
#include <QDebug>

//#include "imageviewerwidget.h"
using namespace white_sdk;

namespace Ui {
class MainWindow;
}

// Тип виджета
enum WIDGET_TYPE
{
    WT_RASTR_IMAGE
};

struct tab_info
{
    tab_info(QWidget *wgt,WIDGET_TYPE type,int id)
    {
        this->wgt=wgt;
        this->type=type;
        this->id=id;
    }
    QWidget *wgt;
    WIDGET_TYPE type;
    int id;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenImage_triggered();


    void on_LayersEditor_triggered();

    void on_SharpenAction_triggered();
    void temp(int,int);
private:
    Ui::MainWindow *ui;
    int m_idCounter;                // Если ведется работа с несколькими проектами
    QTabWidget *m_tabs;             // Виджет вкладок
    QList<tab_info> m_tabs_info;    // Информация
    WRaster *m_image;

};

#endif // MAINWINDOW_H
