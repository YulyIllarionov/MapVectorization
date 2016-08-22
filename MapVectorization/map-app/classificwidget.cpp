#include "stdafx.h"
#include "classificwidget.h"

using namespace SDK_NAMESPACE;

ClassificWidget::ClassificWidget(std::shared_ptr<WRaster> image, ImageViewer *widget, QList<WLayer*> *layers, QWidget *parent) :
    QWidget(parent), m_ui(new Ui::ClassificWidget)
{
    m_ui->setupUi(this);
    m_image = image;
    m_widget = widget;
    m_polygon = NULL;
    for (int i = 0; i < layers->size(); i++)
    {
        WLayer* tlay = layers->at(i);
        if (tlay->getType() == WLayer::LT_TEXT ||
            tlay->getType() == WLayer::LT_LINES)
            m_layers.append(tlay);
    }
    UpdateList();
    if (!m_layers.isEmpty()) m_ui->listWidget->setCurrentRow(0);
    QObject::connect(m_widget->GetPixItem(), SIGNAL(sendCoordAndType(int, int, int)), this, SLOT(GetCoordAndType(int, int, int)));
    m_states = click_selection;

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
    for (int i = 0; i < m_layers.size(); i++)
    {
        WLayer* tlay = m_layers.at(i);
        m_ui->listWidget->addItem(QString(tlay->getName().c_str()));
        m_ui->comboBox->addItem(QString(tlay->getName().c_str()));
    }


}

void ClassificWidget::UpdateCollectionList()
{
    clearCollectionList();
    WLayer* currentLayer = m_layers.at(m_ui->listWidget->currentRow());
    WLayer::LAYER_TYPE type = currentLayer->getType();
    const size_t containerSize = currentLayer->VectorContainerElementsNumber();
    for (int i = 0; i < containerSize; i++)
    {
        if (m_ui->listWidget_2->item(i)->isSelected() || m_ui->listWidget_2->currentRow() == i)
        {
            WVectorObject* vobj = currentLayer->GetVectorElement(i);
            QVector<QPointF> tempPoints;
            for (int j = 0; j < vobj->Length(); j++)
            {
                tempPoints.append(QPointF(vobj->GetPoint(j).GetX(), vobj->GetPoint(j).GetY()));
            }
            switch (type)
            {
            case WLayer::LT_TEXT:
                m_polygonForText.append(m_widget->AddTextSelection(QPolygonF(tempPoints)));
                break;
            case WLayer::LT_LINES:
                QList<QGraphicsRectItem *> lst;
                m_widget->AddLineSelection(tempPoints, lst);
                m_rectForLines.append(lst);
                break;
            }
        }
    }
}

bool ClassificWidget::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate)
        this->setWindowOpacity(1.0);
    if (event->type() == QEvent::WindowDeactivate)
        this->setWindowOpacity(0.5);
    return QWidget::event(event);
}

void ClassificWidget::on_listWidget_currentRowChanged(int currentRow)
{
    m_ui->listWidget_2->clear();
    selectionHistory.clear();
    WLayer* currentLayer = m_layers.at(currentRow);
    //utils::SetTransparent(m_image->m_raster, cv::Mat(m_image->m_raster.size(), CV_8UC1, 1), 10);
    //for(int i=0; i<m_layers.size();i++)
    //{
    //    if (i != currentRow)
    //        utils::SetTransparent(m_image->m_raster, m_layers.at(i)->m_data, 200, 0, true, false);
    //}
    utils::SetTransparent(m_image->m_raster, currentLayer->m_data, 255, 25, true, true);
    m_widget->UpdatePixmap();

    clearCollectionList();
    const size_t containerSize = currentLayer->VectorContainerElementsNumber();
    for (size_t i = 0; i < containerSize; i++)
    {
        switch (m_layers.at(currentRow)->getType())
        {
        case WLayer::LAYER_TYPE_ENUM::LT_TEXT:
        {
            WText* text = dynamic_cast<WText*>(currentLayer->GetVectorElement(i));
            if (text->GetText().empty())
                m_ui->listWidget_2->addItem("!unrecognized!");
            else
                m_ui->listWidget_2->addItem(QString::fromUtf8(text->GetText().c_str()));
            break;
        }
        case WLayer::LAYER_TYPE_ENUM::LT_LINES:
        {
            WLine* line = dynamic_cast<WLine*>(currentLayer->GetVectorElement(i));
            m_ui->listWidget_2->addItem("(" + QString::number(line->m_points.front().x) + "," +
                QString::number(line->m_points.front().y) + ")-(" + QString::number(line->m_points.back().x) +
                "," + QString::number(line->m_points.back().y) + ")  " + QString::number(line->m_points.size()) +
                " points");
            break;
        }
        default:
            break;
        }
    }
}

void ClassificWidget::GetCoordAndType(int x, int y, int type)
{
    if (m_states != nothing)
    {
        if (type == 2)
        {
            if (m_states == polygon_selection)
            {
                delete m_polygon;
                m_polygon = NULL;
                m_ui->ClickSelectionButton->setChecked(false);
                m_ui->PolygonSelectionButton->setChecked(false);
                m_selectPoints.clear();
            }
            if (m_states == click_selection)
            {
                if (!selectionHistory.empty())
                {
                    m_ui->listWidget_2->setItemSelected(m_ui->listWidget_2->item(selectionHistory.back()), false);
                    selectionHistory.pop_back();
                }
            }
        }
        if (type == 1)
        {
            if (m_states == polygon_selection)
            {
                delete m_polygon;
                m_polygon = NULL;
                m_selectPoints.append(QPointF(x, y));
                m_polygon = m_widget->AddSelection(QPolygonF(m_selectPoints));
            }
            if (m_states == click_selection)
            {
                std::vector<int> temp_vector = m_image->DefineObjectsNearPoint(m_layers.at(m_ui->listWidget->currentRow())->getID(), SMapPoint(x, y));
                for (int i = 0; i < temp_vector.size(); i++)
                {
                    m_ui->listWidget_2->setItemSelected(m_ui->listWidget_2->item(temp_vector.at(i)), true);
                    //selectionHistory.push_back(temp_vector.at(i));
                }
                UpdateCollectionList();
            }

        }
        if (type == 9)
        {
            if (m_states == polygon_selection)
            {
                delete m_polygon;
                m_polygon = NULL;
                m_selectPoints.append(QPointF(x, y));
                // Создание вектора
                std::vector<SMapPoint> vec;
                for (int i = 0; i < m_selectPoints.size(); i++)
                {
                    QPointF pf = m_selectPoints.at(i);
                    vec.push_back(SMapPoint((int)pf.x(), (int)pf.y()));
                }
                std::vector<int> temp_vector = m_image->DefineObjectsInsidePolygon(m_layers.at(m_ui->listWidget->currentRow())->getID(), WPolygon(vec));
                for (int i = 0; i < temp_vector.size(); i++)
                {
                    m_ui->listWidget_2->setItemSelected(m_ui->listWidget_2->item(temp_vector.at(i)), 1);
                }
                UpdateCollectionList();
                m_selectPoints.clear();
            }
        }

    }
}

void ClassificWidget::on_listWidget_2_currentRowChanged(int currentRow)
{
    if (currentRow > -1) UpdateCollectionList();

}

void ClassificWidget::on_catLinesButton_clicked()
{
    WLayer* currentLayer = m_layers.at(m_ui->listWidget->currentRow());
    WLine* vobj;
    bool isFirst = true;
    const size_t containerSize = currentLayer->VectorContainerElementsNumber();
    for (size_t i = 0; i < containerSize; i++)
    {
        if (m_ui->listWidget_2->item(i)->isSelected())
        {
            if (isFirst)
            {
                vobj = dynamic_cast<WLine*>(currentLayer->GetVectorElement(i));
                isFirst = false;
            }
            else
            {
                WLine* vobj2 = dynamic_cast<WLine*>(currentLayer->GetVectorElement(i));
                vobj->Concat(*vobj2);
                delete vobj2;
                currentLayer->RemoveVectorElement(i);
            }
        }
    }
}

void ClassificWidget::clearCollectionList()
{
    m_textPolygons.clear();
    for (int i = 0; i < m_polygonForText.size(); i++)
    {
        delete m_polygonForText.at(i);
    }
    m_polygonForText.clear();
    for (int j = 0; j < m_rectForLines.size(); j++)
    {
        for (int i = 0; i < m_rectForLines.at(j).size(); i++)
        {
            delete m_rectForLines.at(j).at(i);
        }
    }
    m_rectForLines.clear();
}

void ClassificWidget::on_ClickSelectionButton_clicked()
{
    m_states = click_selection;
}

void ClassificWidget::on_PolygonSelectionButton_clicked()
{
    m_states = polygon_selection;
}

void ClassificWidget::on_DeleteButton_clicked()
{
    WLayer* currentLayer = m_layers.at(m_ui->listWidget->currentRow());
    std::vector<size_t> idx;
    const size_t containerSize = currentLayer->VectorContainerElementsNumber();
    for (int i = 0; i < containerSize; i++)
    {
        if (m_ui->listWidget_2->item(i)->isSelected())
        {
            idx.push_back(i);
        }
    }
    m_image->CutObjectsFromLayer(m_layers.at(m_ui->listWidget->currentRow())->getID(), idx);
    UpdateCollectionList();

}

void ClassificWidget::on_MoveButton_clicked()
{
    WLayer* currentLayer = m_layers.at(m_ui->listWidget->currentRow());
    std::vector<size_t> idx;
    const size_t containerSize = currentLayer->VectorContainerElementsNumber();
    for (int i = 0; i < containerSize; i++)
    {
        if (m_ui->listWidget_2->item(i)->isSelected())
        {
            idx.push_back(i);
        }
    }
    m_image->PasteObjectsToLayer(m_layers.at(m_ui->comboBox->currentIndex())->getID(),
        m_image->CutObjectsFromLayer(m_layers.at(m_ui->listWidget->currentRow())->getID(), idx));
    UpdateCollectionList();


}
