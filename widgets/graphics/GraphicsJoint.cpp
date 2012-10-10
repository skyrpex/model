#include "GraphicsJoint.h"
#include "model/Joint.h"
#include "model/Transform.h"
#include <QStack>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

static const QColor RootJointColor(240, 170, 255);
static const QColor JointColor(170, 240, 255);
static const QRectF JointRect(-8, -8, 16, 16);
static const QBrush JointBrush(Qt::white);
static const QPen   JointPen(JointColor, 5);

static const QPen LinePen(JointColor, 0);

static const int LabelHorizontalOffset = 1;
static const int LabelVerticalOffset = 10;

GraphicsJoint::GraphicsJoint(Joint *joint, GraphicsJoint *parent) :
    QGraphicsRectItem(JointRect),
    m_joint(joint),
    m_transform(NULL),
    m_parent(parent),
    m_line(new QGraphicsLineItem),
    m_label(new QGraphicsTextItem(this))
{
    setBrush(JointBrush);
    setPen(JointPen);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges | ItemIgnoresTransformations);
    foreach(Joint *child, joint->children())
        m_children.append(new GraphicsJoint(child, this));

    m_line->setPen(LinePen);
    m_line->setZValue(-1);
    m_label->setPos(LabelHorizontalOffset, LabelVerticalOffset);

    setEnabled(false);
}

Joint *GraphicsJoint::joint() const
{
    return m_joint;
}

void GraphicsJoint::setTransform(Transform *transform)
{
    m_transform = transform;
    setEnabled(m_transform);
}

Transform *GraphicsJoint::transform() const
{
    return m_transform;
}

QList<GraphicsJoint *> GraphicsJoint::children() const
{
    return m_children;
}

QGraphicsTextItem *GraphicsJoint::label() const
{
    return m_label;
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

void GraphicsJoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_ASSERT(m_transform);
    QGraphicsRectItem::mouseMoveEvent(event);
    if(m_parent)
    {
        QLineF line(m_parent->pos(), pos());
        m_transform->length = line.length();
        m_transform->rotation = line.angle();
    }
}
