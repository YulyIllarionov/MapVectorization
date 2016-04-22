#include "stdafx.h"

#include "layersviewer.h"

#include "layerconfiguredialog.h"


LayersViewer::LayersViewer(WRaster* image, ImageViewer* widget, QList<WLayer*> *layers,QWidget* parent) :
    QWidget(parent), m_ui(new Ui::LayersViewer), m_widget(widget), m_image(image)
{
    m_ui->setupUi(this);
    m_image = image;
    m_widget = widget;
    UpdateList();
}

LayersViewer::~LayersViewer()
{
    delete m_ui;
}

void LayersViewer::on_Add_clicked()
{
    int num = m_image->GetLayersCount();
    m_tempLayer=m_image->AddLayer();
    m_layers->append(m_tempLayer);
    utils::SetTransparent(m_image->m_raster, cv::Mat(m_image->m_raster.size(), CV_8UC1, 1), 255);
    m_widget->UpdatePixmap();
    LayerConfigureDialog* dlg = new LayerConfigureDialog(m_image, m_widget, m_tempLayer);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowFlags(Qt::WindowStaysOnTopHint);
    QObject::connect(dlg, SIGNAL(Accept()), this, SLOT(UpdateList()));
    QObject::connect(dlg, SIGNAL(Reject()), this, SLOT(onReject()));
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

void LayersViewer::closeEvent(QCloseEvent *event)
{
        utils::SetTransparent(m_image->m_raster, cv::Mat(m_image->m_raster.size(), CV_8UC1, 1), 255);
        m_widget->UpdatePixmap();
}

void LayersViewer::UpdateList()
{
    m_ui->listWidget->clear();

    for (int i = 0; i < m_layers->size(); i++)
    {
        QString temp;
        switch(m_layers->at(i)->getType())
        {
        case WLayer::LAYER_TYPE_ENUM::LT_TEXT:
            temp = "text"; break;
        case WLayer::LAYER_TYPE_ENUM::LT_AREAS:
            temp = "areas"; break;
        case WLayer::LAYER_TYPE_ENUM::LT_LINES:
            temp = "lines"; break;
        case WLayer::LAYER_TYPE_ENUM::LT_OTHER:
            temp = "other"; break;
        case WLayer::LAYER_TYPE_ENUM::LT_NONE:
            temp = "none"; break;
        }

        m_ui->listWidget->addItem(QString::fromStdString(m_layers->at(i)->getName())+" - "+temp);
    }

}
void LayersViewer::on_listWidget_currentRowChanged(int currentRow)
{
    utils::SetTransparent(m_image->m_raster, m_layers->at(currentRow)->m_data,50);
    m_widget->UpdatePixmap();
}

void LayersViewer::onReject()
{
    m_image->RemoveLayer(m_tempLayer->getID());
    m_layers->pop_back();
}