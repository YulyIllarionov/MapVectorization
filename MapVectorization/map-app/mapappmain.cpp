#include "stdafx.h"
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
    delete  m_image;
}

void mapAppMain::on_OpenImage_triggered()
{
    QString str = QFileDialog::getOpenFileName(0, "Open image", "", "Image Files (*.png *.jpg *.bmp) ;; *.*");
    if(!str.isEmpty())
    {
        m_image = new WRaster(str.toStdString());
        ImageViewer*  imView = new ImageViewer(
        QImage((uchar*)m_image->m_raster.data,
          m_image->m_raster.cols, 
            m_image->m_raster.rows,
            m_image->m_raster.step,
          QImage::Format_ARGB32));
        
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
    LayersViewer* viewer = new LayersViewer(m_image, static_cast<ImageViewer*>(m_tabs->currentWidget()),&m_layers);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setWindowFlags(Qt::WindowStaysOnTopHint);
    viewer->show();
}


void mapAppMain::on_SharpenAction_triggered()
{
    bool ok;
    double k = QInputDialog::getDouble(0, "Increase Sharpness", "k  from 0 to 10:", 5, 0.0, 10.0, 2, &ok);
    m_image->IncreaseSharpness(k);
    static_cast<ImageViewer *>(m_tabs->currentWidget())->UpdatePixmap();

}

void mapAppMain::temp(int x, int y)
{
    qDebug() << x << " " << y;
}

void mapAppMain::on_actionBilateral_triggered()
{
    static_cast<ImageViewer *>(m_tabs->currentWidget())->DeletePixmap();
    m_image->SegmentationBilateral();
    static_cast<ImageViewer *>(m_tabs->currentWidget())->UpdatePixmap();
}

void mapAppMain::on_actionMeanshift_triggered()
{
    m_image->SegmentationMeanshift();
    static_cast<ImageViewer *>(m_tabs->currentWidget())->UpdatePixmap();
}

void mapAppMain::on_actionRegimentBlackColor_triggered()
{
    ClassificWidget* viewer = new ClassificWidget(m_image, static_cast<ImageViewer*>(m_tabs->currentWidget()),&m_layers);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setWindowFlags(Qt::WindowStaysOnTopHint);
    viewer->show();
}

void mapAppMain::on_actionUpdatePixmap_triggered()
{
    static_cast<ImageViewer *>(m_tabs->currentWidget())->UpdatePixmap();
}

void mapAppMain::on_SaveAsVector_triggered()
{
    QString str = QFileDialog::getSaveFileName(0, "Save file", "", "Vector Files (*.svg) ;; *.*");
}
