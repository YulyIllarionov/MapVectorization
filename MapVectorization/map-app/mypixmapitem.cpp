#include "stdafx.h"

#include "mypixmapitem.h"

MyPixmapItem::MyPixmapItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{

}

void MyPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit sendCoord(event->pos().x(),event->pos().y());
    emit sendCoordAndType(event->pos().x(), event->pos().y(),event->button());
}

void MyPixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit sendCoordAndType(event->pos().x(), event->pos().y(), event->button()+8);
}

