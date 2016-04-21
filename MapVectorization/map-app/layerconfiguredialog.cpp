#include "stdafx.h"

#include "layerconfiguredialog.h"
#include "ui_layerconfiguredialog.h"


LayerConfigureDialog::LayerConfigureDialog(WRaster* image, ImageViewer* widget,int layerNum,QWidget *parent) :
    QWidget(parent), m_ui(new Ui::LayerConfigureDialog)
{
    m_ui->setupUi(this);
    m_image = image;
    m_widget = widget;
    m_layerNum = layerNum;
    m_firstColor = true;
    QObject::connect(m_widget->GetPixItem(), SIGNAL(sendCoord(int, int)), this, SLOT(GetCoord(int, int)));
    m_ui->LeftSample->installEventFilter(this);
    m_ui->RightSample->installEventFilter(this);
}

LayerConfigureDialog::~LayerConfigureDialog()
{
    delete m_ui;
}

bool LayerConfigureDialog::event(QEvent *event)
{
    if(event->type() == QEvent::WindowActivate)
        this->setWindowOpacity(1.0);
    if(event->type() == QEvent::WindowDeactivate)
        this->setWindowOpacity(0.5);
    return QWidget::event(event);
}

void LayerConfigureDialog::closeEvent(QCloseEvent *event)
{
    utils::SetTransparent(m_image->m_raster, cv::Mat(m_image->m_raster.size(), CV_8UC1, 1), 255);
    m_widget->UpdatePixmap();
}

bool LayerConfigureDialog::eventFilter(QObject *obj, QEvent *event)
{
    /*if(event->type()==QEvent::MouseButtonPress)
    {
        if(obj==m_ui->LeftSample)
        {
           QColor temp_color=QColorDialog::getColor(QColor(m_leftR,m_leftG,m_leftB));
           m_leftR=temp_color.red();
           m_leftG=temp_color.green();
           m_leftB=temp_color.blue();
           UpdateSamples();
           UpdatesMask();
        }
        if(obj==m_ui->RightSample)
        {
           QColor temp_color=QColorDialog::getColor(QColor(m_rightR,m_rightG,m_rightB));
           m_rightR=temp_color.red();
           m_rightG=temp_color.green();
           m_rightB=temp_color.blue();
           UpdateSamples();
           UpdatesMask();
        }

        return true;
    }*/
    return false;
}

void LayerConfigureDialog::GetCoord(int x, int y)
{
    if(m_ui->Pipette->isChecked())
    {
        QColor rgb = m_widget->GetImage().pixel(x,y);
        m_r = rgb.red();
        m_g = rgb.green();
        m_b = rgb.blue();
        QPalette palette;
        palette.setColor(QPalette::Background, QColor(rgb));
        m_ui->SapleFrame->setPalette(palette);
        m_ui->Pipette->setChecked(false);
        activateWindow();
    }
}

void LayerConfigureDialog::UpdateSamples()
{
        QPalette palette;
        w_color tempColor=m_image->m_layers.at(m_layerNum).m_color_range.getLow();
        palette.setColor(QPalette::Background,QColor(tempColor.r, tempColor.g, tempColor.b));
        m_ui->LeftSample->setPalette(palette);
        tempColor = m_image->m_layers.at(m_layerNum).m_color_range.getHigh();
        palette.setColor(QPalette::Background,QColor(tempColor.r, tempColor.g, tempColor.b));
        m_ui->RightSample->setPalette(palette);
}

void LayerConfigureDialog::on_AddColor_clicked()
{
    m_image->AddColorToLayer(m_layerNum, w_color(m_r,m_g,m_b));
    UpdateSamples();
    UpdatesMask();

}

void LayerConfigureDialog::UpdatesMask()
{
    w_color wc2 = m_image->m_layers.at(m_layerNum).m_color_range.getLow();
    w_color wc3= m_image->m_layers.at(m_layerNum).m_color_range.getHigh();
    utils::SetTransparent(m_image->m_raster, m_image->m_layers.at(m_layerNum).m_data, 50);
    m_widget->UpdatePixmap();
    QObject::connect(m_widget->GetPixItem(), SIGNAL(sendCoord(int, int)), this, SLOT(GetCoord(int, int)));
}

void LayerConfigureDialog::on_buttonBox_accepted()
{
    
    m_image->SetLayerType(m_layerNum,(WLayer::LAYER_TYPE)(m_ui->Type->currentIndex()));
    m_image->SetLayerName(m_layerNum, m_ui->Name->text().toStdString());
    emit Accept();

    this->close();

}

void LayerConfigureDialog::on_buttonBox_rejected()
{
    this->close();
}
