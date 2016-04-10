#ifndef LAYERCONFIGUREDIALOG_H
#define LAYERCONFIGUREDIALOG_H

#include <QDialog>

namespace Ui {
class LayerConfigureDialog;
}

class LayerConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LayerConfigureDialog(QWidget *parent = 0);
    ~LayerConfigureDialog();

private:
    Ui::LayerConfigureDialog *ui;
};

#endif // LAYERCONFIGUREDIALOG_H
