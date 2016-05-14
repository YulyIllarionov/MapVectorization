#ifndef CLASSIFICWIDGET_H
#define CLASSIFICWIDGET_H

#include <QWidget>
#include "../sdk/app/sdk_const.h"
#include "../sdk/base_types.h"
#include "../sdk/util/utils.h"
#include "ui_classificwidget.h"
#include <QGraphicsPolygonItem>
#include "imageviewer.h"
using namespace SDK_NAMESPACE;

class ClassificWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClassificWidget(WRaster* image, ImageViewer* widget,QList<WLayer*> *layers,QWidget *parent = 0);
    ~ClassificWidget();
public slots:
    void UpdateList();
protected:
    bool event(QEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void GetCoordAndType(int x, int y, int type);

private:
    Ui::ClassificWidget *     m_ui;
    ImageViewer*              m_widget;
    SDK_NAMESPACE::WRaster*   m_image;
    QList<WLayer*>            m_layers;
    QGraphicsPolygonItem*     m_polygon;
    QVector<QPointF>          m_selectPoints;
};

#endif // CLASSIFICWIDGET_H
