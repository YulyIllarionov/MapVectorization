#ifndef LAYERSDEFINEWIDGET_H
#define LAYERSDEFINEWIDGET_H

#include <QWidget>
#include <QPalette>
#include "imageviewerwidget.h"

namespace Ui {
class LayersDefineWidget;
}

class LayersDefineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LayersDefineWidget(QWidget *parent = 0);
    ~LayersDefineWidget();
    void SetImageWidget(ImageViewerWidget *wgt);
public slots:
    void SetSampleColor(QRgb rgb);
private slots:
    void on_pushButton_Pipette_toggled(bool checked);

private:
    Ui::LayersDefineWidget *ui;
    ImageViewerWidget *wgt;
};

#endif // LAYERSDEFINEWIDGET_H
