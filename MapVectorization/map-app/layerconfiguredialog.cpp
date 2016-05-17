#include "stdafx.h"

#include "layerconfiguredialog.h"
#include "ui_layerconfiguredialog.h"


LayerConfigureDialog::LayerConfigureDialog(WRaster* image, ImageViewer* widget,WLayer *tempLayer,QWidget *parent) :
    QWidget(parent), m_ui(new Ui::LayerConfigureDialog)
{
    m_ui->setupUi(this);
    m_image = image;
    m_widget = widget;
    m_tempLayer = tempLayer;
    QObject::connect(m_widget->GetPixItem(), SIGNAL(sendCoord(int, int)), this, SLOT(GetCoord(int, int)));
    m_ui->SapleFrame->installEventFilter(this);
    m_state=nothing;
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
    if(event->type()==QEvent::MouseButtonPress)
    {
        if (obj == m_ui->SapleFrame)
        {
            QColor temp_color = QColorDialog::getColor(QColor(m_r, m_g, m_b));
            if (temp_color.isValid())
            {
                m_r = temp_color.red();
                m_g = temp_color.green();
                m_b = temp_color.blue();
                on_AddColor_clicked();
            }

            return true;
        }
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
        //m_ui->Pipette->setChecked(false);
        //activateWindow();
        on_AddColor_clicked();
    }
}

void LayerConfigureDialog::UpdateSamples()
{
        QPalette palette;
        w_color tempColor = m_tempLayer->getRange().getLow();
        palette.setColor(QPalette::Background,QColor(tempColor.r, tempColor.g, tempColor.b));
        m_ui->LeftSample->setPalette(palette);
        tempColor = m_tempLayer->getRange().getHigh();
        palette.setColor(QPalette::Background,QColor(tempColor.r, tempColor.g, tempColor.b));
        m_ui->RightSample->setPalette(palette);
}

void LayerConfigureDialog::on_AddColor_clicked()
{
    m_image->AddColorToLayer(m_tempLayer->getID(), w_color(m_r,m_g,m_b));
    UpdateSamples();
    UpdatesMask();

}

void LayerConfigureDialog::UpdatesMask()
{
    w_color wc2 = m_tempLayer->getRange().getLow();
    w_color wc3= m_tempLayer->getRange().getHigh();
    utils::SetTransparent(m_image->m_raster, m_tempLayer->m_data, 50);
    m_widget->UpdatePixmap();
    //QObject::connect(m_widget->GetPixItem(), SIGNAL(sendCoord(int, int)), this, SLOT(GetCoord(int, int)));
}

void LayerConfigureDialog::on_buttonBox_accepted()
{
    QCheckBox * mas[4] = {m_ui->checkBox,m_ui->checkBox_2,m_ui->checkBox_3,m_ui->checkBox_4};
    int tempType = 0;
    for (int i = 0;i < 4;i++)
    {
        if (mas[i]->isChecked()) 
            tempType += 1 << i;
    }
    m_image->SetLayerType(m_tempLayer->getID(),(WLayer::LAYER_TYPE)tempType);
    
    
    m_image->SetLayerName(m_tempLayer->getID(), m_ui->Name->text().toStdString());
    emit Accept();

    this->close();

}

void LayerConfigureDialog::on_buttonBox_rejected()
{
    emit Reject();
    this->close();
}

void LayerConfigureDialog::on_PenButton_toggled(bool checked)
{
    if(checked) m_state=pen;
    else m_state =nothing;
}

void LayerConfigureDialog::on_Pipette_toggled(bool checked)
{
    if(checked) m_state =pen;
    else m_state =nothing;
}

void LayerConfigureDialog::on_EraseButton_toggled(bool checked)
{
    if (checked) m_state = eraser;
    else m_state = nothing;
}
