#include "KeyFrameItem.h"
#include "KeyFrameMime.h"
#include "Constants.h"
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QDebug>

KeyFrameItem::KeyFrameItem(qreal x)
{
    setPixmap(QPixmap(KeyFramePixmap));
    setCacheMode(DeviceCoordinateCache);
    setCursor(Qt::ArrowCursor); // Strange bug fix: the first selected item will always be selected when mouse clicking
    setAcceptedMouseButtons(Qt::LeftButton);
    setPos(x, 0);
}

void KeyFrameItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Strange bug fix: the first selected item will always be selected when mouse clicking
    Q_UNUSED(event);
}

void KeyFrameItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
            .length() < QApplication::startDragDistance())
        return;

    KeyFrameMime *mime = new KeyFrameMime(this);

    QDrag *drag = new QDrag(event->widget());
    drag->setMimeData(mime);
    drag->exec();
}
