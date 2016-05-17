#ifndef MYPIXMAPITEM_H
#define MYPIXMAPITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class MyPixmapItem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit MyPixmapItem(QGraphicsItem *parent = 0);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    sendCoord(int x,int y);
public slots:
};

#endif // MYPIXMAPITEM_H
