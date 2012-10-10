#include "Joint.h"
#include "JointModel.h"
#include "KeyFrames.h"
#include <QDebug>
#include <QStack>

Joint::Joint(Joint *parent) :
    m_model(NULL),
    m_parent(NULL),
    m_isVisible(true)
{
    if(parent)
        parent->addChild(this);
}

Joint::Joint(const QString &name, Joint *parent) :
    m_model(NULL),
    m_parent(NULL),
    m_isVisible(true)
{
    if(parent)
        parent->addChild(this);

    setName(name);
}

Joint::~Joint()
{
    // Remove from the parent and the model
    if(m_parent)
    {
        int i = m_parent->m_children.indexOf(this);
        if(m_model) m_model->beginRemoveJoints(m_parent, i, 1);
        m_parent->m_children.takeAt(i);
        if(m_model) m_model->endRemoveJoints();
    }

    // Delete the children
    foreach(Joint *child, m_children)
    {
        // Unset the parent, so it doesn't mess up our children list
        child->m_parent = NULL;
        delete child;
    }
}


//Joint *Joint::clone() const
//{
//    Joint *joint = new Joint(m_name);

//    foreach(Joint *child, m_children)
//        joint->addChild(child->clone());

//    return joint;
//}

Joint *Joint::parent() const
{
    return m_parent;
}

Joint *Joint::child(int i) const
{
    return m_children.value(i);
}

QList<Joint *> Joint::children() const
{
    return m_children;
}

void Joint::addChild(Joint *child)
{
    insertChild(m_children.count(), child);
}

void Joint::insertChild(int index, Joint *child)
{
    if(index < 0 || index > m_children.count() || !child || child->parent())
        return;

    if(m_model)
        m_model->beginInsertJoints(this, index, 1);
    m_children.insert(index, child);
    child->m_parent = this;
    syncAnims(child);
    QStack<Joint *> stack;
    stack.push(child);
    while(!stack.isEmpty())
    {
        Joint *i = stack.pop();
        i->m_model = m_model;
        foreach(Joint *c, i->children())
            stack.push(c);
    }
    if(m_model) m_model->endInsertJoints();
}

void Joint::removeChild(Joint *child)
{
    takeChild(m_children.indexOf(child));
}

Joint *Joint::takeChild(int i)
{
    if(i < 0 || i >= m_children.count())
        return NULL;

    if(m_model) m_model->beginRemoveJoints(this, i, 1);
    Joint *child = m_children.takeAt(i);
    child->m_parent = NULL;
    QStack<Joint *> stack;
    stack.push(child);
    while(!stack.isEmpty())
    {
        Joint *i = stack.pop();
        i->m_model = NULL;
        foreach(Joint *c, i->children())
            stack.push(c);
    }
    if(m_model) m_model->endRemoveJoints();

    return child;
}

//void Joint::addChildren(const QList<Joint*> &children)
//{
//    insertChildren(m_children.count(), children);
//}

//void Joint::insertChildren(int index, const QList<Joint*> &children)
//{
//    if(index < 0 || index > m_children.count())
//        return;

//    QList<Joint*> itemsToInsert;
//    foreach(Joint *child, children)
//    {
//        if(!child || child->parent())
//            continue;
//        itemsToInsert << child;
//    }


//    if(m_model) m_model->beginInsertJoints(this, index, itemsToInsert.count());
//    for(int n = 0; n < itemsToInsert.count(); ++n)
//    {
//        Joint *child = itemsToInsert.at(n);
//        child->m_parent = this;
//        QStack<Joint *> stack;
//        stack.push(child);
//        while(!stack.isEmpty())
//        {
//            Joint *i = stack.pop();
//            i->m_model = m_model;
//            foreach(Joint *c, i->children())
//                stack.push(c);
//        }
//        m_children.insert(index + n, child);
//    }
//    if(m_model) m_model->endInsertJoints();
//}

//QList<Joint *> Joint::takeChildren()
//{
//    QList<Joint *> removed;
//    if(m_children.count())
//    {
//        if(m_model) m_model->beginRemoveJoints(this, 0, m_children.count());

//        foreach(Joint *child, m_children)
//        {
//            child->m_parent = NULL;
//            QStack<Joint *> stack;
//            stack.push(child);
//            while(!stack.isEmpty())
//            {
//                Joint *i = stack.pop();
//                i->m_model = NULL;
//                foreach(Joint *c, i->children())
//                    stack.push(c);
//            }
//        }
//        removed = m_children;
//        m_children.clear();

//        if(m_model) m_model->endRemoveJoints();
//    }
//    return removed;
//}

QString Joint::name() const
{
    return m_name;
}

void Joint::setName(const QString &name)
{
    m_name = name;
    if(m_model) m_model->emitDataChanged(this, JointModel::NameColumn);
}

bool Joint::isVisible() const
{
    return m_isVisible;
}

void Joint::setVisible(bool visible)
{
    m_isVisible = visible;
}

QMap<Anim *, KeyFrames *> Joint::anims() const
{
    return m_anims;
}

void Joint::syncAnims(Joint *joint) const
{
    foreach(Anim *anim, joint->m_anims.keys())
    {
        if(!m_anims.contains(anim))
        {
            delete joint->m_anims.take(anim);
        }
    }

    foreach(Anim *anim, m_anims.keys())
        if(!joint->m_anims.contains(anim))
            joint->m_anims.insert(anim, new KeyFrames);
}
