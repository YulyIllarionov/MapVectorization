#include "stdafx.h"

#include "mypixmapitem.h"

MyPixmapItem::MyPixmapItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{

}

void MyPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit sendCoord(event->pos().x(),event->pos().y());
}

void MyPixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}

