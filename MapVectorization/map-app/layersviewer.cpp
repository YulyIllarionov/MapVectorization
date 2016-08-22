#include "stdafx.h"

#include "layersviewer.h"

#include "layerconfiguredialog.h"


LayersViewer::LayersViewer(std::shared_ptr<WRaster> image, ImageViewer* widget, QList<WLayer*> *layers,QWidget* parent) :
    QWidget(parent), m_ui(new Ui::LayersViewer)
{
    m_ui->setupUi(this);
    m_image = image;
    m_widget = widget;
    m_layers = layers;
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
        /*QString temp;
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
        }*/
        QString temp= QString::number((int)m_layers->at(i)->getType());
        m_ui->listWidget->addItem(QString::fromStdString(m_layers->at(i)->getName())+" - "+temp);
    }

}
void LayersViewer::on_listWidget_currentRowChanged(int currentRow)
{
    if (m_layers->size() > 0 &&currentRow>-1)
    {
        utils::SetTransparent(m_image->m_raster, m_layers->at(currentRow)->m_data, 255, 25, true, true);
        m_widget->UpdatePixmap();
        if(m_layers->at(currentRow)->getType()==(WLayer::LAYER_TYPE_ENUM::LT_LINES | WLayer::LAYER_TYPE_ENUM::LT_TEXT))
        {
           m_ui->SplitButton->setEnabled(true);
        }
        else m_ui->SplitButton->setEnabled(false);

        if(m_layers->at(currentRow)->getType()==(WLayer::LAYER_TYPE_ENUM::LT_TEXT)||
                m_layers->at(currentRow)->getType()==(WLayer::LAYER_TYPE_ENUM::LT_LINES))
        {
           m_ui->VectorizationButton->setEnabled(true);
        }
        else m_ui->VectorizationButton->setEnabled(false);


    }
}

void LayersViewer::onReject()
{
    m_image->RemoveLayer(m_tempLayer->getID());
    m_layers->pop_back();
}
void LayersViewer::on_Remove_clicked()
{
    m_image->RemoveLayer(m_layers->at(m_ui->listWidget->currentRow())->getID());
    m_layers->removeAt(m_ui->listWidget->currentRow());
    UpdateList();
}

void LayersViewer::on_SavePngButton_clicked()
{
    int index;

    if((index=m_ui->listWidget->currentRow())>-1)
    {
        QString str = QFileDialog::getSaveFileName(0, "Save layer", QString(m_layers->at(index)->getName().c_str()) + ".png", "Image Files (*.png *.jpg *.bmp) ;; *.*");
        if (!str.isEmpty())
        {
            cv::Mat toSave;
            cv::normalize(m_layers->at(index)->m_data, toSave, 0, 255, cv::NORM_MINMAX, CV_8UC1);
            //cv::imshow("1", toSave);
            //cv::waitKey(0);
            cv::imwrite(str.toStdString(), toSave);
        }
    }
}

void LayersViewer::on_SplitButton_clicked()
{
    LayerIDs vec;
    m_image->SplitLayer(m_layers->at(m_ui->listWidget->currentRow())->getID(),vec);
    for (int i = 0;i < vec.size();i++)
    {
        m_layers->append(m_image->GetLayerById(vec.at(i)));
    }
    UpdateList();

}

void LayersViewer::on_VectorizationButton_clicked()
{
    m_layers->at(m_ui->listWidget->currentRow())->InicializeVectorContainer();
}
