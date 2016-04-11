#include "layersviewer.h"
#include "ui_layersviewer.h"

LayersViewer::LayersViewer(WRaster *image, ImageViewer *wgt, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayersViewer)
{
    ui->setupUi(this);
    m_image=image;
    m_wgt=wgt;

}

LayersViewer::~LayersViewer()
{
    delete ui;
}

void LayersViewer::on_Add_clicked()
{
    LayerConfigureDialog *dlg=new LayerConfigureDialog(QImage((uchar*) m_image->m_raster.data,
                                                              m_image->m_raster.cols, m_image->m_raster.rows, m_image->m_raster.step, QImage::Format_RGB888));

    QObject::connect(m_wgt->GetPixItem(),SIGNAL(sendCoord(int,int)),dlg ,SLOT(GetCoord(int,int)));
    dlg->show();
}
