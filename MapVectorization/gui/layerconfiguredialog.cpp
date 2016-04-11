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

void LayerConfigureDialog::focusInEvent(QFocusEvent *)
{
    this->setWindowOpacity(1.0);
}

void LayerConfigureDialog::focusOutEvent(QFocusEvent *)
{
    this->setWindowOpacity(0.5);
}

void LayerConfigureDialog::GetCoord(int x, int y)
{
    qDebug()<<x<<" "<<y;
    QPalette palette;
    palette.setColor(QPalette::Background ,QColor(m_image.pixel(x,y)));
    ui->SapleFrame->setPalette(palette);
    ui->Pipette->setChecked(false);
}
