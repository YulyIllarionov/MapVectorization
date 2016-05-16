#include "imageviewer.h"

ImageViewer::ImageViewer(QImage image, QWidget *parent) : QGraphicsView(parent)
{
    m_image=image;
    m_pixItem=NULL;
    setScene(&m_scn);
}

void ImageViewer::ZoomIn()
{
    this->scale(1.1,1.1);
}

void ImageViewer::ZoomOut()
{
    this->scale(0.9,0.9);
}

void ImageViewer::FitView()
{
    fitInView(m_scn.itemsBoundingRect() ,Qt::KeepAspectRatio);
}

void ImageViewer::SetImage(const QImage &image)
{
    m_image = image;
}

void ImageViewer::UpdatePixmap()
{
    delete m_pixItem;
    m_pixItem=new MyPixmapItem();
    m_pixItem->setPixmap(QPixmap::fromImage(m_image));
    m_scn.addItem(m_pixItem);
}

MyPixmapItem *ImageViewer::GetPixItem() const
{
    return m_pixItem;
}

