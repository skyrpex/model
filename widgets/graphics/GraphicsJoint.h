#ifndef GRAPHICSJOINT_H
#define GRAPHICSJOINT_H

#include <QGraphicsRectItem>
#include <QList>

class Joint;
class QGraphicsLineItem;
class QGraphicsTextItem;
class Transform;

class GraphicsJoint : public QGraphicsRectItem
{
public:
    GraphicsJoint(Joint *joint, GraphicsJoint *parent = NULL);

    void setTransform(Transform *transform);
    Transform *transform() const;

    QGraphicsTextItem *label() const;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    Joint *m_joint;
    Transform *m_transform;

    GraphicsJoint *m_parent;
    QList<GraphicsJoint *> m_children;
    QGraphicsLineItem *m_line;
    QGraphicsTextItem *m_label;
};

#endif // GRAPHICSJOINT_H
