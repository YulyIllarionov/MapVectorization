#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Настройка вкладок
    m_tabs=new QTabWidget();
    m_tabs->setTabsClosable(true);
    ui->centralWidget->layout()->addWidget(m_tabs);
    ui->centralWidget->layout()->setMargin(0);

}

MainWindow::~MainWindow()
{
    delete m_tabs;
    delete m_image;
    delete ui;
}

void MainWindow::on_OpenImage_triggered()
{
    QString str=QFileDialog::getOpenFileName(0,"Открыть изображение","","Image Files (*.png *.jpg *.bmp) ;; *.*");
    if(!str.isEmpty())
    {
        m_image=new WRaster(str.toStdString());
        ImageViewer *imView=new ImageViewer(QImage((uchar*) m_image->m_raster.data,
                                                   m_image->m_raster.cols, m_image->m_raster.rows, m_image->m_raster.step, QImage::Format_RGB888));
        m_tabs->addTab(imView,str.section('/', -1, -1));
        m_tabs_info.append(tab_info(imView,WT_RASTR_IMAGE,m_idCounter++));
        imView->UpdatePixmap();
        imView->FitView();
        QObject::connect(ui->ZoomIn,SIGNAL(triggered(bool)),imView,SLOT(ZoomIn()));
        QObject::connect(ui->ZoomOut,SIGNAL(triggered(bool)),imView,SLOT(ZoomOut()));
        QObject::connect(ui->FitImage,SIGNAL(triggered(bool)),imView,SLOT(FitView()));
    }
}

void MainWindow::on_LayersEditor_triggered()
{
    LayersViewer *viewer=new LayersViewer(m_image,static_cast<ImageViewer *>(m_tabs->currentWidget()));
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}



//    LayersDefineWidget *lwgt=new LayersDefineWidget();
//    lwgt->setAttribute(Qt::WA_DeleteOnClose);
//    lwgt->SetImageWidget(static_cast<ImageViewerWidget *>(tabs->currentWidget()));
//    lwgt->show();


void MainWindow::on_SharpenAction_triggered()
{
    bool ok;
    double k=QInputDialog::getDouble(0,"Увеличение резкости","k - от 0 до 10:",5,0.0,10.0,2,&ok);
}

void MainWindow::temp(int x, int y)
{
    qDebug()<<x<<" "<<y;
}
