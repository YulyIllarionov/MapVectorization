#ifndef LAYERSVIEWER_H
#define LAYERSVIEWER_H

#include <QWidget>
#include "layerconfiguredialog.h"
#include "base_types.h"
#include "imageviewer.h"
using namespace white_sdk;

namespace Ui {
class LayersViewer;
}

class LayersViewer : public QWidget
{
    Q_OBJECT

public:
    explicit LayersViewer(WRaster *image,ImageViewer *wgt,QWidget *parent = 0);
    ~LayersViewer();
protected:
    bool event(QEvent *event);
private slots:
    void on_Add_clicked();

private:
    Ui::LayersViewer *ui;
    ImageViewer *m_wgt;
    WRaster *m_image;
};

#endif // LAYERSVIEWER_H
