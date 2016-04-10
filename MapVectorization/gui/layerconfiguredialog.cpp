#include "layerconfiguredialog.h"
#include "ui_layerconfiguredialog.h"

LayerConfigureDialog::LayerConfigureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LayerConfigureDialog)
{
    ui->setupUi(this);
}

LayerConfigureDialog::~LayerConfigureDialog()
{
    delete ui;
}
