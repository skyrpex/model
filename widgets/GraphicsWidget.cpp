#include "GraphicsWidget.h"
#include "model/JointModel.h"
#include "widgets/graphics/GraphicsJoint.h"
#include <QxGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>

GraphicsWidget::GraphicsWidget(JointModel *model, QWidget *parent) :
    QWidget(parent),
    m_view(new QxGraphicsView(new QGraphicsScene, this))
{
    m_view->scene()->addItem(new GraphicsJoint(model->root()));

    QVBoxLayout *l = new QVBoxLayout(this);
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(m_view);
}

void GraphicsWidget::setCurrentAnim(int i)
{
}

void GraphicsWidget::setCurrentFrame(int frame)
{
}
