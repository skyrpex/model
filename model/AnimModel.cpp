#include "AnimModel.h"
#include "Anim.h"
#include "JointModel.h"
#include <QDebug>

static const char *NameColumnText = "Name";
static const char *FrameCountColumnText = "Frame count";
static const char *FpsColumnText = "Frames per second";

AnimModel::AnimModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

AnimModel::~AnimModel()
{
    if(!m_animations.isEmpty())
    {
        beginRemoveRows(QModelIndex(), 0, m_animations.count()-1);
        foreach(Anim *anim, m_animations)
        {
            anim->m_model = NULL;
            delete anim;
        }
        m_animations.clear();
        endRemoveRows();
    }
}

QList<Anim *> AnimModel::anims() const
{
    return m_animations;
}

void AnimModel::addAnim(Anim *anim)
{
    insertAnim(m_animations.count(), anim);
}

void AnimModel::insertAnim(int row, Anim *anim)
{
    if(row < 0 || row > m_animations.count() || anim->m_model)
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_animations.insert(row, anim);
    anim->m_model = this;
    endInsertRows();
}

void AnimModel::removeAnim(Anim *anim)
{
    takeAnim(m_animations.indexOf(anim));
}

Anim *AnimModel::takeAnim(int row)
{
    if(row < 0 || row >= m_animations.count())
        return NULL;

    beginRemoveRows(QModelIndex(), row, row);
    Anim *anim = m_animations.takeAt(row);
    anim->m_model = NULL;
    endRemoveRows();

    return anim;
}

QVariant AnimModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= m_animations.count())
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Anim *anim = m_animations.at(index.row());
        switch(index.column())
        {
        case NameColumn: return anim->m_name;
        case FrameCountColumn: return anim->m_frameCount;
        case FpsColumn: return anim->m_fps;
        default: return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags AnimModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QModelIndex AnimModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || row >= m_animations.count() || column < 0)
        return QModelIndex();

    return createIndex(row, column, m_animations.at(row));
}

QVariant AnimModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch(section)
    {
    case NameColumn: return tr(NameColumnText);
    case FrameCountColumn: return tr(FrameCountColumnText);
    case FpsColumn: return tr(FpsColumnText);
    default: return QVariant();
    }
}

int AnimModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()) return m_animations.count();

    return 0;
}

int AnimModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

bool AnimModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(role != Qt::EditRole)
        return false;

    Anim *anim = static_cast<Anim *>(index.internalPointer());
    switch(index.column())
    {
    case NameColumn:
        anim->setName(value.toString());
        emit dataChanged(index, index);
        return true;

    case FrameCountColumn:
        anim->setFrameCount(value.toInt());
        emit dataChanged(index, index);
        return true;

    case FpsColumn:
        anim->setFps(value.toInt());
        emit dataChanged(index, index);
        return true;

    default:
        return false;
    }
}

void AnimModel::emitAnimChanged(Anim *anim, int column)
{
    int i = m_animations.indexOf(anim);
    if(i == -1) return;

    QModelIndex idx = index(i, column, QModelIndex());
    emit dataChanged(idx, idx);
}
