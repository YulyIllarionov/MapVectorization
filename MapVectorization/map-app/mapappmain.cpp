﻿#include "stdafx.h"
#include "mapappmain.h"

using namespace SDK_NAMESPACE;

mapAppMain::mapAppMain(QWidget *parent)
    : QMainWindow(parent)
{
    m_ui.setupUi(this);
    // Настройка вкладок
    m_tabs = new QTabWidget();
    m_tabs->setTabsClosable(true);
    m_ui.centralWidget->layout()->addWidget(m_tabs);
    m_ui.centralWidget->layout()->setMargin(0);
}

mapAppMain::~mapAppMain()
{

}

void mapAppMain::on_OpenImage_triggered()
{
    QString str = QFileDialog::getOpenFileName(0, "Открыть изображение", "", "Image Files (*.png *.jpg *.bmp) ;; *.*");
    if(!str.isEmpty())
    {
        m_image = WRaster(str.toStdString());
        ImageViewer*  imView = new ImageViewer(
          QImage((uchar*) m_image.m_raster.data,
          m_image.m_raster.cols, 
          m_image.m_raster.rows, 
          m_image.m_raster.step, 
          QImage::Format_RGB888));
        
        m_tabs->addTab(imView, str.section('/', -1, -1));
        m_tabsInfo.append(tab_info(imView, WT_RASTR_IMAGE, ++m_idCounter));
        imView->UpdatePixmap();
        imView->FitView();
        QObject::connect(m_ui.ZoomIn, SIGNAL(triggered(bool)), imView, SLOT(ZoomIn()));
        QObject::connect(m_ui.ZoomOut, SIGNAL(triggered(bool)), imView, SLOT(ZoomOut()));
        QObject::connect(m_ui.FitImage, SIGNAL(triggered(bool)), imView, SLOT(FitView()));
    }
}

void mapAppMain::on_LayersEditor_triggered()
{
    LayersViewer* viewer = new LayersViewer(&m_image, static_cast<ImageViewer*>(m_tabs->currentWidget()));
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setWindowFlags(Qt::WindowStaysOnTopHint);
    viewer->show();
}


void mapAppMain::on_SharpenAction_triggered()
{
    bool ok;
    double k = QInputDialog::getDouble(0, "Увеличение резкости", "k - от 0 до 10:", 5, 0.0, 10.0, 2, &ok);
}

void mapAppMain::temp(int x, int y)
{
    qDebug() << x << " " << y;
}
