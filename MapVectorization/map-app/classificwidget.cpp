#include "stdafx.h"
#include "classificwidget.h"

using namespace SDK_NAMESPACE;

ClassificWidget::ClassificWidget(WRaster *image, ImageViewer *widget,QList<WLayer*> *layers, QWidget *parent) :
    QWidget(parent), m_ui(new Ui::ClassificWidget)
{
    m_ui->setupUi(this);
    m_image = image;
    m_widget = widget;
    m_polygon=NULL;
    for(int i=0;i<layers->size();i++)
    {
        WLayer* tlay=layers->at(i);
        if(tlay->getType()==WLayer::LT_TEXT||
                tlay->getType()==WLayer::LT_LINES||
                tlay->getType()==WLayer::LT_AREAS)
        m_layers.append(tlay);
    }
    if(!m_layers.isEmpty()) m_ui->listWidget->setCurrentRow(0);
    QObject::connect(m_widget->GetPixItem(), SIGNAL(sendCoordAndType(int,int,int)), this, SLOT(GetCoordAndType(int,int,int)));
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
    delete m_polygon;
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

bool ClassificWidget::event(QEvent *event)
{
    if(event->type() == QEvent::WindowActivate)
        this->setWindowOpacity(1.0);
    if(event->type() == QEvent::WindowDeactivate)
        this->setWindowOpacity(0.5);
    return QWidget::event(event);
}

void ClassificWidget::on_listWidget_currentRowChanged(int currentRow)
{
    utils::SetTransparent(m_image->m_raster, cv::Mat(m_image->m_raster.size(), CV_8UC1, 1), 50);
    for(int i=0; i<m_layers.size();i++)
    {
        if(i!=currentRow)    
        utils::SetTransparent(m_image->m_raster, m_layers.at(i)->m_data, 150,0,1,0);
    }
    utils::SetTransparent(m_image->m_raster, m_layers.at(currentRow)->m_data, 255, 0, 1, 0);
    m_widget->UpdatePixmap();

    WObjectContainer &cont=m_layers.at(currentRow)->m_objects;
    for(int i=0;i<cont.size();i++)
    {
        m_ui->listWidget_2->addItem("�"+QString::number(i));
    }
}

void ClassificWidget::GetCoordAndType(int x, int y, int type)
{
    if(m_ui->lasso->isChecked())
    {
        if(type==2)
        {
            delete m_polygon;
            m_polygon = NULL;
            m_ui->lasso->setChecked(false);
            m_selectPoints.clear();
        }
        if(type==1)
        {
            delete m_polygon;
            m_polygon = NULL;
            m_selectPoints.append(QPointF(x,y));
            m_polygon=m_widget->AddSelection(QPolygonF(m_selectPoints));
        }
        if (type == 9)
        {
            delete m_polygon;
            m_polygon = NULL;
            m_selectPoints.append(QPointF(x, y));
            // �������� �������
            std::vector<SMapPoint> vec;
            for (int i = 0;i < m_selectPoints.size();i++)
            {
                QPointF pf= m_selectPoints.at(i);
                vec.push_back(SMapPoint((int)pf.x(), (int)pf.y()));
            }
            m_image->DeleteOblectsFromLayer(m_layers.at(m_ui->listWidget->currentRow())->getID(), WPolygon(vec));
            on_listWidget_currentRowChanged(m_ui->listWidget->currentRow());
        }

    }
}

void ClassificWidget::on_listWidget_2_currentRowChanged(int currentRow)
{
    WObjectContainer &cont=m_layers.at(m_ui->listWidget->currentRow())->m_objects;
    WVectorObject &vobj=cont.at(currentRow);
    WLayer::LAYER_TYPE type=m_layers.at(m_ui->listWidget->currentRow())->getType();
    
    
    QVector<QPointF> tempPoints;
    for (int i = 0;i<vobj.Length();i++)
    {
        tempPoints.append(QPointF(vobj.GetPoint(i).GetX(), vobj.GetPoint(i).GetY()));
    }

    switch(type)
    {
        case WLayer::LT_TEXT:
            m_polygonForText.append(m_widget->AddTextSelection(QPolygonF(tempPoints)));
            break;
        case WLayer::LT_LINES:
            QList<QGraphicsRectItem *> lst;
            m_widget->AddLineSelection(tempPoints,lst);
            m_rectForLines.append(lst);
        break;
    }

}
