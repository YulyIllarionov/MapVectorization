#include "stdafx.h"

#include "layersviewer.h"

#include "layerconfiguredialog.h"

using namespace SDK_NAMESPACE;

LayersViewer::LayersViewer(WRaster* image, ImageViewer* widget, QWidget* parent) :
    QWidget(parent), m_ui(new Ui::LayersViewer), m_widget(widget), m_image(image)
{
    m_ui->setupUi(this);
}

LayersViewer::~LayersViewer()
{
    delete m_ui;
}

void LayersViewer::on_Add_clicked()
{
    LayerConfigureDialog* dlg = new LayerConfigureDialog(
      QImage((uchar*) m_image->m_raster.data,
      m_image->m_raster.cols, 
      m_image->m_raster.rows, 
      m_image->m_raster.step, 
      QImage::Format_ARGB32));

    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowFlags(Qt::WindowStaysOnTopHint);
    QObject::connect(m_widget->GetPixItem(), SIGNAL(sendCoord(int, int)), dlg, SLOT(GetCoord(int, int)));
    dlg->show();
}

bool LayersViewer::event(QEvent *event)
{
    if(event->type() == QEvent::WindowActivate)
        this->setWindowOpacity(1.0);
    if(event->type() == QEvent::WindowDeactivate)
        this->setWindowOpacity(0.5);
    return QWidget::event(event);
}
