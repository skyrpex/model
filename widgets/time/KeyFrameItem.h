#ifndef KEYFRAMEITEM_H
#define KEYFRAMEITEM_H

#include <QGraphicsPixmapItem>

class KeyFrameItem : public QGraphicsPixmapItem
{
public:
    KeyFrameItem(qreal x = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // KEYFRAMEITEM_H
