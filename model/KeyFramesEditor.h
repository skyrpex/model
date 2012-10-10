#ifndef KEYFRAMESEDITOR_H
#define KEYFRAMESEDITOR_H

#include <QGraphicsView>
#include <QList>
#include "KeyFrames.h"

class QGraphicsPixmapItem;
class QGraphicsLineItem;
class KeyFrameItem;

class KeyFramesEditor : public QGraphicsView
{
    Q_OBJECT
public:
    explicit KeyFramesEditor(QWidget *parent = 0);

    QSize sizeHint() const;

    void setData(KeyFrameMap *keyFrames, int frameCount, int currentFrame);

signals:
    void currentFrameChanged(int frame);

public slots:
    void setCurrentFrame(int frame);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    QPointF keyFramePos(const QPoint &pos, bool bound = true) const;
    int frameAt(const QPoint &pos, bool bound = true) const;
    int frameAt(const QPointF &scenePos, bool bound = true) const;

    KeyFrameMap *m_keyFrames;
    int m_frameCount;
    QList<KeyFrameItem *> m_keyFrameItems;
    QGraphicsPixmapItem *m_dragItem;
    QGraphicsLineItem *m_lineItem;
};

#endif // KEYFRAMESEDITOR_H
