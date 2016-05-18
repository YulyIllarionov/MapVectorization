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
    m_states=nothing;
}
void ClassificWidget::closeEvent(QCloseEvent *event)
{
        utils::SetTransparent(m_image->m_raster, cv::Mat(m_image->m_raster.size(), CV_8UC1, 1), 255);
        m_widget->UpdatePixmap();
        clearCollectionList();
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

void ClassificWidget::UpdateCollectionList()
{
    clearCollectionList();
    WObjectContainer &cont=m_layers.at(m_ui->listWidget->currentRow())->m_objects;
    WLayer::LAYER_TYPE type=m_layers.at(m_ui->listWidget->currentRow())->getType();

    for(int i=0;i<cont.size();i++)
    {
        if(m_ui->listWidget_2->item(i)->isSelected()|| m_ui->listWidget_2->currentRow()==i)
        {
            WVectorObject &vobj=cont.at(i);
            QVector<QPointF> tempPoints;
            for (int j = 0;j<vobj.Length();j++)
            {
                tempPoints.append(QPointF(vobj.GetPoint(j).GetX(), vobj.GetPoint(j).GetY()));
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
    m_ui->listWidget_2->clear();
    utils::SetTransparent(m_image->m_raster, cv::Mat(m_image->m_raster.size(), CV_8UC1, 1), 50);
    for(int i=0; i<m_layers.size();i++)
    {
        if(i!=currentRow)    
        utils::SetTransparent(m_image->m_raster, m_layers.at(i)->m_data, 150,0,1,0);
    }
    utils::SetTransparent(m_image->m_raster, m_layers.at(currentRow)->m_data, 255, 0, 1, 0);
    m_widget->UpdatePixmap();

    clearCollectionList();
    WObjectContainer &cont=m_layers.at(currentRow)->m_objects;
    for(int i=0;i<cont.size();i++)
    {
        m_ui->listWidget_2->addItem("#"+QString::number(i));
    }
}

void ClassificWidget::GetCoordAndType(int x, int y, int type)
{
    if(!m_states)
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
            // Создание вектора
            std::vector<SMapPoint> vec;
            for (int i = 0;i < m_selectPoints.size();i++)
            {
                QPointF pf= m_selectPoints.at(i);
                vec.push_back(SMapPoint((int)pf.x(), (int)pf.y()));
            }

            if(m_states==lasso_delete)
            m_image->DeleteOblectsFromLayer(m_layers.at(m_ui->listWidget->currentRow())->getID(), WPolygon(vec));

            if(m_states==lasso_move)
            m_image->CopyObjectsToAnotherLayer(m_layers.at(m_ui->listWidget->currentRow())->getID(),m_layers.at(m_ui->comboBox->currentIndex())->getID(),WPolygon(vec));

            m_ui->listWidget_2->clear();
            m_selectPoints.clear();
            //UpdateCollectionList();
        }

    }
}

void ClassificWidget::on_listWidget_2_currentRowChanged(int currentRow)
{
   UpdateCollectionList();

}

void ClassificWidget::on_catLinesButton_clicked()
{
    WObjectContainer &cont=m_layers.at(m_ui->listWidget->currentRow())->m_objects;
    WLine* vobj;
    bool isFirst=true;
    for(int i=0;i<cont.size();i++)
    {
        if(m_ui->listWidget_2->item(i)->isSelected())
        {
            if(isFirst)
            {
                vobj = dynamic_cast<WLine*>(&cont.at(i));
                isFirst=false;
            }
            else
            {
                WLine* vobj2 = dynamic_cast<WLine*>(&cont.at(i));
                //vobj->Concat(*vobj2);
            }
        }
    }
}

void ClassificWidget::clearCollectionList()
{
    m_textPolygons.clear();
        for(int i=0;i<m_polygonForText.size();i++)
        {
            delete m_polygonForText.at(i);
        }
        m_polygonForText.clear();
        for(int j=0;j<m_rectForLines.size();j++)
        {
            for(int i=0;i<m_rectForLines.at(j).size();i++)
            {
                delete m_rectForLines.at(j).at(i);
            }
        }
        m_rectForLines.clear();
}

void ClassificWidget::on_lasso_clicked()
{
    if(m_ui->lasso->isChecked())
    {
        m_states=nothing;
        m_ui->lasso->setChecked(false);
    }
    else
    {
        m_states=lasso_delete;
    }
}

void ClassificWidget::on_lassoMove_clicked()
{
    if(m_ui->lassoMove->isChecked())
    {
        m_states=nothing;
        m_ui->lassoMove->setChecked(false);
    }
    else
    {
        m_states=lasso_move;
    }
}
