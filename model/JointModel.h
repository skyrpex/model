#ifndef JOINTMODEL_H
#define JOINTMODEL_H

#include <QAbstractItemModel>

class Joint;
class AnimModel;

class JointModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Column
    {
        NameColumn = 0,
        VisibleColumn = 1,
        AnimColumn = 2
    };
    enum Role
    {
        AnimRole = Qt::UserRole + 10
    };

    explicit JointModel(QObject *parent = 0);

    Joint *root() const;
    AnimModel *animModel() const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private slots:
    void onAnimsInserted(const QModelIndex &parent, int first, int last);
    void onAnimsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onAnimsChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    friend class AnimModel;
    friend class Joint;

    QModelIndex indexOf(Joint *joint, int column) const;

    void beginInsertJoints(Joint *parent, int row, int count);
    void endInsertJoints();
    void beginRemoveJoints(Joint *parent, int row, int count);
    void endRemoveJoints();

    void emitDataChanged(Joint *joint, int column);

    AnimModel *m_animModel;
    Joint *m_root;
};

#endif // JOINTMODEL_H
