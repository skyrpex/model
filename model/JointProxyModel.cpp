#include "JointProxyModel.h"
#include "JointModel.h"

JointProxyModel::JointProxyModel(JointModel *model, QObject *parent) :
    QSortFilterProxyModel(parent),
    m_showVisible(true),
    m_showAnims(true),
    m_showAnimID(-1)
{
    setSourceModel(model);
}

void JointProxyModel::showVisibleColumn(bool show)
{
    m_showVisible = show;
    invalidateFilter();
}

void JointProxyModel::showAnims(bool show)
{
    m_showAnims = show;
    invalidateFilter();
}

void JointProxyModel::showAnim(int id)
{
    m_showAnimID = id;
    invalidateFilter();
}

bool JointProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(!QSortFilterProxyModel::filterAcceptsColumn(source_column, source_parent)) return false;

    switch(source_column)
    {
    case JointModel::NameColumn: return true;
    case JointModel::VisibleColumn: return m_showVisible;
    default:
        if(!m_showAnims) return false;
        if(m_showAnimID <= -1) return true;
        return ((m_showAnimID + JointModel::AnimColumn) == source_column);
        break;
    }
}
