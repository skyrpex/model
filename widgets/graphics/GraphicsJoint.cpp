#include "GraphicsJoint.h"
#include "model/Joint.h"
#include <QStack>
#include <QGraphicsScene>

static const QRectF JointRect(-5, -5, 10, 10);

GraphicsJoint::GraphicsJoint(Joint *joint, GraphicsJoint *parent) :
    QGraphicsRectItem(JointRect),
    m_joint(joint),
    m_parent(parent),
    m_line(new QGraphicsLineItem)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
    foreach(Joint *child, joint->children())
        m_children.append(new GraphicsJoint(child, this));
}

QVariant GraphicsJoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionHasChanged)
    {
        if(m_parent) m_line->setLine(QLineF(pos(), m_parent->pos()));
        foreach(GraphicsJoint *child, m_children)
            child->m_line->setLine(QLineF(pos(), child->pos()));
    }
    else if(change == ItemSceneChange)
    {
        if(scene())
        {
            foreach(GraphicsJoint *child, m_children)
                scene()->removeItem(child);
            scene()->removeItem(m_line);
        }
    }
    else if(change == ItemSceneHasChanged)
    {
        if(scene())
        {
            foreach(GraphicsJoint *child, m_children)
                scene()->addItem(child);
            scene()->addItem(m_line);
        }
    }
    return QGraphicsRectItem::itemChange(change, value);
}
