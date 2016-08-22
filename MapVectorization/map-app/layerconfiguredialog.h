#ifndef LAYERCONFIGUREDIALOG_H
#define LAYERCONFIGUREDIALOG_H

#include <QWidget>
#include <QDebug>
#include <QColorDialog>

#include <memory>

#include "../sdk/app/sdk_const.h"
#include "../sdk/base_types.h"
#include "../sdk/util/utils.h"

#include "imageviewer.h"
using namespace SDK_NAMESPACE;

namespace Ui {
  class LayerConfigureDialog;
}

class LayerConfigureDialog : public QWidget
{
    Q_OBJECT

public:
    explicit LayerConfigureDialog(std::shared_ptr<WRaster> image, ImageViewer* widget,WLayer *tempLayer, QWidget *parent = 0);
    ~LayerConfigureDialog();

protected:
    bool event(QEvent *event);
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
public slots:
    void GetCoord(int x,int y);
    void UpdateSamples();
    void UpdatesMask();

private slots:
    void on_AddColor_clicked();
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_PenButton_toggled(bool checked);

    void on_Pipette_toggled(bool checked);

    void on_EraseButton_toggled(bool checked);

signals:
    void Accept();
    void Reject();
private:
    Ui::LayerConfigureDialog*   m_ui;
    ImageViewer*              m_widget;
    std::shared_ptr<WRaster>  m_image;
    WLayer *m_tempLayer;

    int   m_r, m_g, m_b;
    enum States
    {
        nothing,pipette,pen,eraser
    };

    States m_state;
};

#endif // LAYERCONFIGUREDIALOG_H
