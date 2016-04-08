#include "layersdefinewidget.h"
#include "ui_layersdefinewidget.h"

LayersDefineWidget::LayersDefineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayersDefineWidget)
{
    ui->setupUi(this);
}

LayersDefineWidget::~LayersDefineWidget()
{
    delete ui;
}

void LayersDefineWidget::SetImageWidget(ImageViewerWidget *wgt)
{
    this->wgt=wgt;
}

void LayersDefineWidget::SetSampleColor(QRgb rgb)
{
    QPalette palette;
    palette.setColor(QPalette::Background ,QColor(rgb) );
    ui->frame->setPalette( palette );

    ui->pushButton_Pipette->setChecked(false);
    QObject::disconnect(wgt,SIGNAL(sendPointColor(QRgb)),this,SLOT(SetSampleColor(QRgb)));
    wgt->SetState(def);
}

void LayersDefineWidget::on_pushButton_Pipette_toggled(bool checked)
{
    if(wgt!=NULL&&checked)
    {
          QObject::connect(wgt,SIGNAL(sendPointColor(QRgb)),this,SLOT(SetSampleColor(QRgb)));
          wgt->SetState(pipette);
    }
}
