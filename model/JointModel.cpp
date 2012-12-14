#include "JointModel.h"
#include "AnimModel.h"
#include "Joint.h"
#include "Anim.h"
#include "KeyFrames.h"
#include "Transform.h"
#include <QStack>
#include <QDebug>

JointModel::JointModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_animModel(new AnimModel(this)),
    m_root(new Joint)
{
    m_root->m_model = this;
    m_root->m_name = tr("Root");

    connect(m_animModel, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(onAnimsInserted(QModelIndex,int,int)));
    connect(m_animModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), SLOT(onAnimsAboutToBeRemoved(QModelIndex,int,int)));
    connect(m_animModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(onAnimsChanged(QModelIndex,QModelIndex)));
}

Joint *JointModel::root() const
{
    return m_root;
}

AnimModel *JointModel::animModel() const
{
    return m_animModel;
}

QModelIndex JointModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || column < 0) return QModelIndex();

    if(!parent.isValid())
    {
        if(row == 0) return createIndex(0, column, m_root);
        return QModelIndex();
    }

    Joint *parentJoint = static_cast<Joint *>(parent.internalPointer());
    if(row >= parentJoint->m_children.count()) return QModelIndex();

    Joint *joint = parentJoint->m_children.at(row);
    return createIndex(row, column, joint);
}

QModelIndex JointModel::parent(const QModelIndex &index) const
{
    if(!index.isValid()) return QModelIndex();

    Joint *joint = static_cast<Joint *>(index.internalPointer());
    if(joint == m_root) return QModelIndex();

    Joint *parentJoint = joint->m_parent;
    if(parentJoint == m_root) return createIndex(0, 0, m_root);

    Joint *gp = parentJoint->m_parent;
    int row = gp->m_children.indexOf(parentJoint);
    return createIndex(row, 0, parentJoint);
}

int JointModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()) return 1;

    if(parent.column() != 0) return 0;

    Joint *joint = static_cast<Joint *>(parent.internalPointer());
    return joint->m_children.count();
}

int JointModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return AnimColumn + m_animModel->anims().count();
}

QVariant JointModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();

    Joint *joint = static_cast<Joint *>(index.internalPointer());
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
        case NameColumn: return joint->name();
        case VisibleColumn: return joint->isVisible();
        default:
        {
            int i = index.column() - AnimColumn;
            if(0 <= i  && i < m_animModel->anims().count())
            {
//                return m_animModel->anims().at(i)->name();
                Anim *anim = m_animModel->anims().at(i);
                KeyFrames *keyFrames = joint->m_anims.value(anim, NULL);
                return QVariant::fromValue(keyFrames);
            }

        }
        }
    }
    else if(role == AnimRole)
    {
        int i = index.column() - AnimColumn;
        if(0 <= i  && i < m_animModel->anims().count())
            return QVariant::fromValue(m_animModel->anims().at(i));
    }

    return QVariant();
}

bool JointModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()) return false;

    Joint *joint = static_cast<Joint *>(index.internalPointer());
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
        case NameColumn: joint->setName(value.toString()); return true;
        case VisibleColumn: joint->setVisible(value.toBool()); return true;
        default: return false;
        }
    }

    return false;
}

Qt::ItemFlags JointModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) return false;

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant JointModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(section)
        {
        case NameColumn: return tr("Name");
        case VisibleColumn: return tr("Visible");
        default:
        {
            int i = section - AnimColumn;
            if(0 <= i  && i < m_animModel->anims().count())
                return m_animModel->anims().at(i)->name();

        }
        }
    }

    return QVariant();
}

void JointModel::onAnimsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);

    emit layoutAboutToBeChanged();

    beginInsertColumns(QModelIndex(), JointModel::AnimColumn+first, JointModel::AnimColumn+last);
    QStack<Joint *> stack;
    stack.push(m_root);
    while(!stack.isEmpty())
    {
        Joint *joint = stack.pop();
        for(int i = first; i <= last; ++i)
        {
            Anim *anim = m_animModel->anims().at(i);
            joint->m_anims.insert(anim, new KeyFrames);
        }

        foreach(Joint *child, joint->children())
            stack.push(child);
    }
    endInsertColumns();

    emit layoutChanged();
}

void JointModel::onAnimsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    beginRemoveColumns(QModelIndex(), JointModel::AnimColumn+first, JointModel::AnimColumn+last);
    QStack<Joint *> stack;
    stack.push(m_root);
    while(!stack.isEmpty())
    {
        Joint *joint = stack.pop();
        for(int i = first; i <= last; ++i)
            delete joint->m_anims.take(m_animModel->anims().at(i));

        foreach(Joint *child, joint->children())
            stack.push(child);
    }
    endRemoveColumns();
}

void JointModel::onAnimsChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    emit layoutAboutToBeChanged();
    for(int i = topLeft.row(); i <= bottomRight.row(); ++i)
    {
        Anim *anim = m_animModel->anims().at(i);

        QStack<Joint *> stack;
        stack.push(m_root);
        while(!stack.isEmpty())
        {
            Joint *joint = stack.pop();
            KeyFrameMap *keyFrames = joint->m_anims.value(anim)->data;
            KeyFrameMap::Iterator it = keyFrames->begin();
            while(it != keyFrames->end())
            {
                if(it.key() < anim->frameCount())
                    ++it;
                else
                {
                    delete it.value();
                    it = keyFrames->erase(it);
                }
            }

            QModelIndex index = indexOf(joint, AnimColumn+i);
            emit dataChanged(index, index);

            foreach(Joint *child, joint->children())
                stack.push(child);
        }
    }
    emit layoutChanged();
}

QModelIndex JointModel::indexOf(Joint *joint, int column) const
{
    // Invalid model or joint
    if(!joint) return QModelIndex();

    // Root joint
    if(joint == m_root) return createIndex(0, column, m_root);

    // Get the child indexes
    QList<int> childIndexes;
    Joint *currentJoint = joint;
    while(currentJoint)
    {
        if(currentJoint->parent())
        {
            int row = currentJoint->parent()->children().indexOf(currentJoint);
            if(row < 0)
                return QModelIndex();

            childIndexes << row;
        }
        else
            childIndexes << 0;

        currentJoint = currentJoint->parent();
    }

    // Build the index
    QModelIndex idx;
    for(int i = childIndexes.count() - 1; i >= 0 ; --i)
        idx = index(childIndexes.at(i), column, idx);
    return idx;
}

void JointModel::beginInsertJoints(Joint *parent, int row, int count)
{
    beginInsertRows(indexOf(parent, 0), row, row + count - 1);
}

void JointModel::endInsertJoints()
{
    endInsertRows();
}

void JointModel::beginRemoveJoints(Joint *parent, int row, int count)
{
    beginRemoveRows(indexOf(parent, 0), row, row + count - 1);
}

void JointModel::endRemoveJoints()
{
    endRemoveRows();
}

void JointModel::emitDataChanged(Joint *joint, int column)
{
    QModelIndex index = indexOf(joint, column);
    if(!index.isValid()) return;

    emit dataChanged(index, index);
}
