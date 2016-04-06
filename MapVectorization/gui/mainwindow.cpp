#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Настройка вкладок
    tabs=new QTabWidget();
    tabs->setTabsClosable(true);
    ui->centralWidget->layout()->addWidget(tabs);
    ui->centralWidget->layout()->setMargin(0);
}

MainWindow::~MainWindow()
{
    delete tabs;
    delete ui;
}

void MainWindow::on_OpenImage_triggered()
{
    QString str=QFileDialog::getOpenFileName(0,"Открыть изображение","","Image Files (*.png *.jpg *.bmp) ;; *.*");
    if(!str.isEmpty())
    {
        ImageViewerWidget *temp=new ImageViewerWidget(str);
        tabs->addTab(temp,str.section('/', -1, -1));
        QObject::connect(ui->actionZoom_in,SIGNAL(triggered(bool)),temp,SLOT(zoomIn()));
        QObject::connect(ui->actionZoom_out,SIGNAL(triggered(bool)),temp,SLOT(zoomOut()));
        temp->setNormalSizeOfImage();

    }
}

void MainWindow::on_actionSharpen_triggered()
{
    bool ok;
    double k=QInputDialog::getDouble(0,"Увеличение резкости","k - от 0 до 10:",5,0.0,10.0,2,&ok);
}
