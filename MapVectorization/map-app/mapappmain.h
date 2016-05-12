#ifndef MAPAPPMAIN_H
#define MAPAPPMAIN_H

#include <QMainWindow>

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>

#include <QDebug>

#include "ui_mapappmain.h"

#include "../sdk/app/sdk_const.h"
#include "../sdk/base_types.h"

#include "imageviewer.h"
#include "layersviewer.h"
#include "classificwidget.h"


// Тип виджета
enum WIDGET_TYPE
{
    WT_RASTR_IMAGE
};

struct tab_info
{
    tab_info(QWidget* widget, WIDGET_TYPE type, int id)
      : widget(widget), type(type), id(id)
    {
    }
    QWidget*    widget;
    WIDGET_TYPE type;
    int         id;
};

class mapAppMain : public QMainWindow
{
    Q_OBJECT

public:
    mapAppMain(QWidget* parent = 0);
    ~mapAppMain();

private slots:
    void on_OpenImage_triggered();
    void on_LayersEditor_triggered();
    void on_SharpenAction_triggered();
    void temp(int,int);

    void on_actionBilateral_triggered();

    void on_actionMeanshift_triggered();

    void on_actionRegimentBlackColor_triggered();

private:
    Ui::mapAppMainClass       m_ui;
    int                       m_idCounter;  // Если ведется работа с несколькими проектами
    QTabWidget*               m_tabs;       // Виджет вкладок
    QList<tab_info>           m_tabsInfo;   // Информация
    SDK_NAMESPACE::WRaster    *m_image;
    QList<WLayer *>           m_layers;

private:
    
};

#endif // MAPAPPMAIN_H
