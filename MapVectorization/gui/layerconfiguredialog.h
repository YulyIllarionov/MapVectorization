#ifndef LAYERCONFIGUREDIALOG_H
#define LAYERCONFIGUREDIALOG_H

#include <QWidget>
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
public slots:
    void GetCoord(int x,int y);
private:
    Ui::LayerConfigureDialog *ui;
    QImage m_image;
};

#endif // LAYERCONFIGUREDIALOG_H
