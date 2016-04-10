#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QGraphicsView>
#include <QImage>
#include "mypixmapitem.h"

class ImageViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageViewer(QImage image,QWidget *parent = 0);
    MyPixmapItem *GetPixItem() const;
    void SetImage(const QImage &image);

signals:

public slots:
    void ZoomIn();
    void ZoomOut();
    void FitView();
    void UpdatePixmap();
private:
     QGraphicsScene m_scn;
     MyPixmapItem *m_pixItem;
     QImage m_image;
};

#endif // IMAGEVIEWER_H
