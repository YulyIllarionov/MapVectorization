#ifndef LAYERCONFIGUREDIALOG_H
#define LAYERCONFIGUREDIALOG_H

#include <QWidget>
#include <QDebug>
#include <QColorDialog>


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
    explicit LayerConfigureDialog(WRaster* image, ImageViewer* widget, QWidget *parent = 0);
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

signals:
    void Accept();

private:
    Ui::LayerConfigureDialog*   m_ui;
    ImageViewer*              m_widget;
    WRaster*   m_image;

    int   m_leftR, m_leftG, m_leftB, m_rightR, m_rightG, m_rightB;
    int   m_r, m_g, m_b;
    bool  m_firstColor;
};

#endif // LAYERCONFIGUREDIALOG_H