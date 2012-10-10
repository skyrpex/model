#ifndef GRAPHICSJOINT_H
#define GRAPHICSJOINT_H

#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QList>

class Joint;

class GraphicsJoint : public QGraphicsRectItem
{
public:
    GraphicsJoint(Joint *joint, GraphicsJoint *parent = NULL);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    Joint *m_joint;

    GraphicsJoint *m_parent;
    QList<GraphicsJoint *> m_children;
    QGraphicsLineItem *m_line;
};

#endif // GRAPHICSJOINT_H
