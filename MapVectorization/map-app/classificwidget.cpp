#include "stdafx.h"
#include "classificwidget.h"

using namespace SDK_NAMESPACE;

ClassificWidget::ClassificWidget(WRaster *image, ImageViewer *widget,QList<WLayer*> *layers, QWidget *parent) :
    QWidget(parent), m_ui(new Ui::ClassificWidget)
{
    m_ui->setupUi(this);
    m_image = image;
    m_widget = widget;
    for(int i=0;i<layers->size();i++)
    {
        WLayer* tlay=layers->at(i);
        if(tlay->getType()==WLayer::LT_TEXT||
                tlay->getType()==WLayer::LT_LINES||
                tlay->getType()==WLayer::LT_AREAS)
        m_layers.append(tlay);
    }
    if(!m_layers.isEmpty()) m_ui->listWidget->setCurrentRow(0);
    UpdateList();
}
void ClassificWidget::closeEvent(QCloseEvent *event)
{
        utils::SetTransparent(m_image->m_raster, cv::Mat(m_image->m_raster.size(), CV_8UC1, 1), 255);
        m_widget->UpdatePixmap();
}

ClassificWidget::~ClassificWidget()
{
    delete m_ui;
}

void ClassificWidget::UpdateList()
{
    m_ui->listWidget->clear();
    m_ui->comboBox->clear();
    for(int i=0;i<m_layers.size();i++)
    {
        WLayer* tlay=m_layers.at(i);
        m_ui->listWidget->addItem(QString(tlay->getName().c_str()));
        m_ui->comboBox->addItem(QString(tlay->getName().c_str()));
    }
}

void ClassificWidget::on_listWidget_currentRowChanged(int currentRow)
{
    utils::SetTransparent(m_image->m_raster, cv::Mat(m_image->m_raster.size(), CV_8UC1, 1), 50);
    for(int i=0; i<m_layers.size();i++)
    {
        if(i!=currentRow) ;
            // Здесь нужно просуммировать слои
            //utils::SetTransparent(m_image->m_raster, m_layers->at(i)->m_data, 150);
    }

}
