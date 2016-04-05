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
        temp->setNormalSizeOfImage();

    }
}
