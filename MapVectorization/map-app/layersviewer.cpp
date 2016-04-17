#include "stdafx.h"

#include "layersviewer.h"

#include "layerconfiguredialog.h"


LayersViewer::LayersViewer(WRaster* image, ImageViewer* widget, QWidget* parent) :
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
    LayerConfigureDialog* dlg = new LayerConfigureDialog(m_image, m_widget);

    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowFlags(Qt::WindowStaysOnTopHint);
    QObject::connect(dlg, SIGNAL(Accept()), this, SLOT(UpdateList()));
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
    utils::SetTransparent(m_image->m_raster,cv::Mat(), 255);
    m_widget->UpdatePixmap();
}
void LayersViewer::UpdateList()
{
    m_ui->listWidget->clear();
    for (LayersContainer::iterator it = m_image->m_layers.begin(); it != m_image->m_layers.end(); ++it)
    {
        QString temp;
        switch(it->m_type)
        {
        case WLayer::LAYER_TYPE::LT_TEXT_AND_LINES:
            temp = "text_and_lines"; break;
        case WLayer::LAYER_TYPE::LT_AREAS:
            temp = "areas"; break;
        }
        m_ui->listWidget->addItem(QString::fromStdString(it->m_name)+" - "+temp);
    }

}
void LayersViewer::on_listWidget_currentRowChanged(int currentRow)
{
    utils::SetTransparent(m_image->m_raster, m_image->m_layers.at(currentRow).m_data,50);
    m_widget->UpdatePixmap();
}
