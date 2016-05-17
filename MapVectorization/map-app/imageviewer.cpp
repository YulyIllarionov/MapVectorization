#include "stdafx.h"

#include "imageviewer.h"

ImageViewer::ImageViewer(QImage image, QWidget *parent) : QGraphicsView(parent), img(1, 1, QImage::Format_ARGB32)
{
    m_image = image;
    m_pixItem = NULL;
    setScene(&m_scene);
    m_pixItem = new MyPixmapItem();
    m_scene.addItem(m_pixItem);
    img.fill(QColor(qRgba(255, 255, 255, 255)));
    m_pixItem->setPixmap(QPixmap::fromImage(m_image));
}

void ImageViewer::ZoomIn()
{
    this->scale(1.1, 1.1);
}

void ImageViewer::ZoomOut()
{
    this->scale(0.9, 0.9);
}

void ImageViewer::FitView()
{
    fitInView(m_scene.itemsBoundingRect() ,Qt::KeepAspectRatio);
}

void ImageViewer::SetImage(const QImage &image)
{
    m_image = image;
}

void ImageViewer::UpdatePixmap()
{
    //delete m_pixItem;
    //m_pixItem = new MyPixmapItem();
    //m_scene.removeItem(m_pixItem);
   
    m_pixItem->setPixmap(QPixmap::fromImage(img));
    m_pixItem->setPixmap(QPixmap::fromImage(m_image));
    //m_scene.addItem(m_pixItem);
}
QImage& ImageViewer::GetImage()
{
    return m_image;
}

QGraphicsPolygonItem* ImageViewer::AddSelection(QPolygonF polygon)
{
    QPen pen(Qt::DashLine);
    QBrush brush(QColor(17,107,209,100));
    return scene()->addPolygon(polygon,pen,brush);
}


MyPixmapItem* ImageViewer::GetPixItem() const
{
    return m_pixItem;
}
void ImageViewer::DeletePixmap()
{
    //delete m_pixItem;
    //m_pixItem = NULL;
    m_pixItem->setPixmap(QPixmap::fromImage(img));
}

