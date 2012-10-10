#ifndef JOINTPROXYMODEL_H
#define JOINTPROXYMODEL_H

#include <QSortFilterProxyModel>

class JointModel;

class JointProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit JointProxyModel(JointModel *model, QObject *parent = 0);

public slots:
    void showVisibleColumn(bool show);
    void showAnims(bool show);
    void showAnim(int id);
    
protected:
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;

private:
    bool m_showVisible;
    bool m_showAnims;
    int m_showAnimID;
};

#endif // JOINTPROXYMODEL_H
