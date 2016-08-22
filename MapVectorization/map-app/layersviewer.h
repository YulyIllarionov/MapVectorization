#ifndef LAYERSVIEWER_H
#define LAYERSVIEWER_H

#include <QWidget>
#include "ui_layersviewer.h"

#include "../sdk/app/sdk_const.h"
#include "../sdk/base_types.h"
#include "../sdk/util/utils.h"
#include <memory>

#include "imageviewer.h"
using namespace SDK_NAMESPACE;

class LayersViewer : public QWidget
{
    Q_OBJECT

public:
    explicit LayersViewer(std::shared_ptr<WRaster> image, ImageViewer* widget, QList<WLayer*> *layers, QWidget* parent = 0);
    ~LayersViewer();
    public slots:
    void UpdateList();
protected:
    bool event(QEvent *event);
    void closeEvent(QCloseEvent *event);
    private slots:
    void on_Add_clicked();
    void onReject();
    void on_listWidget_currentRowChanged(int currentRow);

    void on_Remove_clicked();

    void on_SavePngButton_clicked();

    void on_SplitButton_clicked();

    void on_VectorizationButton_clicked();

private:
    Ui::LayersViewer         *m_ui;
    ImageViewer              *m_widget;
    std::shared_ptr<WRaster>  m_image;
    QList<WLayer*>           *m_layers;
    WLayer                   *m_tempLayer;
};
#endif // LAYERSVIEWER_H
