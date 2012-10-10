#ifndef ANIMMODEL_H
#define ANIMMODEL_H

#include <QAbstractListModel>

class Anim;
class JointModel;

class AnimModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Column
    {
        NameColumn,
        FrameCountColumn,
        FpsColumn,
        ColumnCount
    };

    explicit AnimModel(QObject *parent);
    ~AnimModel();

    QList<Anim *> anims() const;

    void addAnim(Anim *anim);
    void insertAnim(int row, Anim *anim);
    void removeAnim(Anim *anim);
    Anim *takeAnim(int row);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    friend class Anim;

    void emitAnimChanged(Anim *anim, int column);

    QList<Anim *> m_animations;
};

#endif // ANIMMODEL_H
