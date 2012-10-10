#include "GraphicsWidget.h"
#include "model/JointModel.h"
#include "model/Joint.h"
#include "model/KeyFrames.h"
#include "model/Transform.h"
#include "model/AnimModel.h"
#include "widgets/graphics/GraphicsJoint.h"
#include <QxGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QDebug>
#include <QStack>

GraphicsWidget::GraphicsWidget(JointModel *model, QWidget *parent) :
    QWidget(parent),
    m_model(model),
    m_view(new QxGraphicsView(new QGraphicsScene, this)),
    m_root(new GraphicsJoint(model->root())),
    m_currentAnim(NULL),
    m_currentFrame(0)
{
    QVBoxLayout *l = new QVBoxLayout(this);
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(m_view);

    m_view->scene()->addItem(m_root);
}

void GraphicsWidget::setCurrentAnim(int i)
{
    qDebug() << __FUNCTION__ << i;
    m_currentAnim = m_model->animModel()->anims().value(i);
    setCurrentFrame(0);
}

void GraphicsWidget::setCurrentFrame(int frame)
{
    qDebug() << __FUNCTION__ << frame;
    m_currentFrame = frame;

    QStack<GraphicsJoint *> stack;
    stack.push(m_root);
    while(!stack.isEmpty())
    {
        GraphicsJoint *gjoint = stack.pop();
        Joint *joint = gjoint->joint();
        KeyFrames *keyFrames = joint->anims().value(m_currentAnim);
        Transform *transform = keyFrames? keyFrames->data->value(m_currentFrame) : NULL;
        gjoint->setTransform(transform);
        foreach(GraphicsJoint *gchild, gjoint->children())
            stack.push(gchild);
    }
}
