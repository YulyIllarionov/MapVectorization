#include "imageviewerwidget.h"
#include "ui_imageviewerwidget.h"

ImageViewerWidget::ImageViewerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageViewerWidget)
{
    ui->setupUi(this);
    setup();
}

ImageViewerWidget::ImageViewerWidget(QString filename, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ImageViewerWidget)
{
    ui->setupUi(this);
    setup();
    cv_image = cv::imread(filename.toStdString(), CV_LOAD_IMAGE_COLOR);
    image=QImage((uchar*) cv_image.data, cv_image.cols, cv_image.rows, cv_image.step, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(image));
}

ImageViewerWidget::~ImageViewerWidget()
{
    delete label;
    delete ui;
}


void ImageViewerWidget::zoomIn()
{
    if((label->pixmap()->width()>label->width()*1.25)&&(label->pixmap()->height()>label->height()*1.25))
        scaleImage(1.25);
}

void ImageViewerWidget::zoomOut()
{
        scaleImage(0.8);
}

void ImageViewerWidget::setNormalSizeOfImage()
{
    if(label->pixmap()->width()>label->pixmap()->height())
    {
        int temp_width=ui->scrollArea->width()-ui->scrollArea->verticalScrollBar()->width();
        label->resize(temp_width,
                      label->pixmap()->height()*temp_width/label->pixmap()->width());
    }
    else
    {
        int temp_height=ui->scrollArea->height()-ui->scrollArea->horizontalScrollBar()->height();
        label->resize(label->pixmap()->width()*temp_height/label->pixmap()->height(),
                      temp_height);
    }
}
void ImageViewerWidget::scaleImage(double factor)
{
    label->resize(factor * label->size());

}

void ImageViewerWidget::setup()
{
    installEventFilter(this);
    _state=zoom;
    label=new QLabel();
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label->setScaledContents(true);
    ui->scrollArea->setWidget(label);
}
