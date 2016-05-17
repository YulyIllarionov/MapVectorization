#ifndef LAYERCONFIGUREDIALOG_H
#define LAYERCONFIGUREDIALOG_H

#include <QWidget>
#include <QDebug>
#include <QRgb>
#include "base_types.h"
using namespace white_sdk;

namespace Ui {
class LayerConfigureDialog;
}

class LayerConfigureDialog : public QWidget
{
    Q_OBJECT

public:
    explicit LayerConfigureDialog(QImage image, QWidget *parent = 0);
    ~LayerConfigureDialog();
protected:
    bool event(QEvent *event);
public slots:
    void GetCoord(int x,int y);
    void UpdateSamples();
private slots:
    void on_AddColor_clicked();

private:
    Ui::LayerConfigureDialog *ui;
    QImage m_image;
    int m_leftR,m_leftG,m_leftB,m_rightR,m_rightG,m_rightB;
    int m_r,m_g,m_b;
    bool m_firstColor;
};

#endif // LAYERCONFIGUREDIALOG_H
