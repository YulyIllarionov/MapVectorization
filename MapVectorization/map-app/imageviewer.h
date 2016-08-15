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
    MyPixmapItem* GetPixItem() const;
    void SetImage(const QImage &image);
    QImage& GetImage();
    QGraphicsPolygonItem *AddSelection(QPolygonF polygon);
    QGraphicsPolygonItem *AddTextSelection(QPolygonF polygon);
    void AddLineSelection(QVector<QPointF> &vec, QList<QGraphicsRectItem *> &items);
    void ClearObjects();

signals:

public slots:
    void ZoomIn();
    void ZoomOut();
    void FitView();
    void UpdatePixmap();
    void DeletePixmap();

private:
     QGraphicsScene     m_scene;
     MyPixmapItem*      m_pixItem;
     QImage             m_image;
     QImage             img;
     QList<QPointF>                          m_selectPoints;
     QList<QPolygonF>                        m_textPolygons;
     QList<QGraphicsPolygonItem*>            m_polygonForText;
     QList< QList<QGraphicsRectItem*> >      m_rectForLines;

};

#endif // IMAGEVIEWER_H
