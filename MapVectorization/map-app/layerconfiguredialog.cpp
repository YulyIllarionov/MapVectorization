#include "stdafx.h"

#include "layerconfiguredialog.h"
#include "ui_layerconfiguredialog.h"


LayerConfigureDialog::LayerConfigureDialog(WRaster* image, ImageViewer* widget,QWidget *parent) :
    QWidget(parent), m_ui(new Ui::LayerConfigureDialog)
{
    m_ui->setupUi(this);
    m_image = image;
    m_widget = widget;
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
    utils::SetTransparent(m_image->m_raster,cv::Mat(), 255);
    m_widget->UpdatePixmap();
}

bool LayerConfigureDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress)
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
    }
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
        //activateWindow();
    }
}

void LayerConfigureDialog::UpdateSamples()
{
    if(!m_firstColor)
    {
        QPalette palette;
        palette.setColor(QPalette::Background, QColor(m_leftR, m_leftG, m_leftB));
        m_ui->LeftSample->setPalette(palette);
        palette.setColor(QPalette::Background, QColor(m_rightR, m_rightG, m_rightB));
        m_ui->RightSample->setPalette(palette);
    }
}

void LayerConfigureDialog::on_AddColor_clicked()
{
    if(m_firstColor)
    {
        m_rightR = m_leftR = m_r;
        m_rightG = m_leftG = m_g;
        m_rightB = m_leftB = m_b;
        m_firstColor = false;
    }
    else
    {
        if(m_r > m_rightR) m_rightR = m_r;
        else if (m_r < m_leftR) m_leftR = m_r;

        if(m_g > m_rightG) m_rightG = m_g;
        else if (m_g < m_leftG) m_leftG = m_g;

        if(m_b > m_rightB) m_rightB = m_b;
        else if (m_r < m_leftB) m_leftB = m_b;
    }
    UpdateSamples();
    UpdatesMask();


}

void LayerConfigureDialog::UpdatesMask()
{
    int number = m_image->m_layers.size();
    m_image->AddLayer();
    std::vector<uchar> bgr;
    bgr.push_back(m_leftB);
    bgr.push_back(m_rightB);
    bgr.push_back(m_leftG);
    bgr.push_back(m_rightG);
    bgr.push_back(m_leftR);
    bgr.push_back(m_rightR);
    m_image->SetLayerMask(number, bgr);

    utils::SetTransparent(m_image->m_raster, m_image->m_layers.at(number).m_data, 50);
    m_widget->UpdatePixmap();
    QObject::connect(m_widget->GetPixItem(), SIGNAL(sendCoord(int, int)), this, SLOT(GetCoord(int, int)));
    m_image->m_layers.pop_back();
}

void LayerConfigureDialog::on_buttonBox_accepted()
{
    int number = m_image->m_layers.size();
    m_image->AddLayer();
    std::vector<uchar> bgr;
    bgr.push_back((m_rightB + m_leftB) / 2);
    bgr.push_back((m_rightG + m_leftG) / 2);
    bgr.push_back((m_rightR + m_leftR) / 2);
    bgr.clear();

    bgr.push_back(m_leftB);
    bgr.push_back(m_rightB);
    bgr.push_back(m_leftG);
    bgr.push_back(m_rightG);
    bgr.push_back(m_leftR);
    bgr.push_back(m_rightR);

    m_image->SetLayerMask(number, bgr);

    
    m_image->SetLayerType(number,(WLayer::LAYER_TYPE)(m_ui->Type->currentIndex()));
    m_image->SetLayerName(number, m_ui->Name->text().toStdString());
    emit Accept();

    this->close();

}

void LayerConfigureDialog::on_buttonBox_rejected()
{
    this->close();
}
