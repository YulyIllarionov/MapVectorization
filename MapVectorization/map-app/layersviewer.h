#ifndef LAYERSVIEWER_H
#define LAYERSVIEWER_H

#include <QWidget>

#include "ui_layersviewer.h"

#include "../sdk/app/sdk_const.h"
#include "../sdk/base_types.h"

#include "imageviewer.h"


class LayersViewer : public QWidget
{
    Q_OBJECT

public:
    explicit LayersViewer(SDK_NAMESPACE::WRaster* image, ImageViewer* widget, QWidget* parent = 0);
    ~LayersViewer();
protected:
    bool event(QEvent *event);
private slots:
    void on_Add_clicked();

private:
    Ui::LayersViewer*         m_ui;
    ImageViewer*              m_widget;
    SDK_NAMESPACE::WRaster*   m_image;
};

#endif // LAYERSVIEWER_H
