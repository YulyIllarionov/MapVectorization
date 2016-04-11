#include "layerconfiguredialog.h"
#include "ui_layerconfiguredialog.h"

LayerConfigureDialog::LayerConfigureDialog(QImage image,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerConfigureDialog)
{
    ui->setupUi(this);
    m_image=image;
}

LayerConfigureDialog::~LayerConfigureDialog()
{
    delete ui;
}

void LayerConfigureDialog::GetCoord(int x, int y)
{
    QPalette palette;
    palette.setColor(QPalette::Background ,QColor(m_image.pixel(x,y)));
    ui->SapleFrame->setPalette(palette);
    ui->Pipette->setChecked(false);
}
