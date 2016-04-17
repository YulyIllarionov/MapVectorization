#ifndef LAYERSVIEWER_H
#define LAYERSVIEWER_H

#include <QWidget>
#include "ui_layersviewer.h"

#include "../sdk/app/sdk_const.h"
#include "../sdk/base_types.h"
#include "../sdk/util/utils.h"

#include "imageviewer.h"
using namespace SDK_NAMESPACE;

class LayersViewer : public QWidget
{
    Q_OBJECT

public:
    explicit LayersViewer(WRaster* image, ImageViewer* widget, QWidget* parent = 0);
    ~LayersViewer();
public slots:
    void UpdateList();
protected:
    bool event(QEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:
    void on_Add_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::LayersViewer*         m_ui;
    ImageViewer*              m_widget;
    SDK_NAMESPACE::WRaster*   m_image;
};

#endif // LAYERSVIEWER_H
