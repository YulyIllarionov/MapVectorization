#include "layerconfiguredialog.h"
#include "ui_layerconfiguredialog.h"

LayerConfigureDialog::LayerConfigureDialog(QImage image,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerConfigureDialog)
{
    ui->setupUi(this);
    m_image=image;
    m_firstColor=true;
}

LayerConfigureDialog::~LayerConfigureDialog()
{
    delete ui;
}

bool LayerConfigureDialog::event(QEvent *event)
{
    if(event->type()==QEvent::WindowActivate)
        this->setWindowOpacity(1.0);
    if(event->type()==QEvent::WindowDeactivate)
        this->setWindowOpacity(0.5);
    return QWidget::event(event);
}

void LayerConfigureDialog::GetCoord(int x, int y)
{
    if(ui->Pipette->isChecked())
    {
        QColor rgb=m_image.pixel(x,y);
        m_r=rgb.red();
        m_g=rgb.green();
        m_b=rgb.blue();
        QPalette palette;
        palette.setColor(QPalette::Background ,QColor(rgb));
        ui->SapleFrame->setPalette(palette);
        ui->Pipette->setChecked(false);
        activateWindow();
    }
}

void LayerConfigureDialog::UpdateSamples()
{
    if(!m_firstColor)
    {
        QPalette palette;
        palette.setColor(QPalette::Background ,QColor(m_leftR,m_leftG,m_leftB));
        ui->LeftSample->setPalette(palette);
        palette.setColor(QPalette::Background ,QColor(m_rightR,m_rightG,m_rightB));
        ui->RightSample->setPalette(palette);
    }
}

void LayerConfigureDialog::on_AddColor_clicked()
{
    if(m_firstColor)
    {
        m_rightR=m_leftR=m_r;
        m_rightG=m_leftG=m_g;
        m_rightB=m_leftB=m_b;
        m_firstColor=false;
    }
    else
    {
        if(m_r>m_rightR) m_rightR=m_r;
        else if (m_r<m_leftR) m_leftR=m_r;

        if(m_g>m_rightG) m_rightG=m_g;
        else if (m_g<m_leftG) m_leftG=m_g;

        if(m_b>m_rightB) m_rightB=m_b;
        else if (m_r<m_leftB) m_leftB=m_b;
    }
    UpdateSamples();

}
