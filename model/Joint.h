#ifndef JOINT_H
#define JOINT_H

#include <QList>
#include <QString>
#include <QMap>

class JointModel;
class Anim;
class KeyFrames;

class Joint
{
public:
    explicit Joint(Joint *parent = 0);
    explicit Joint(const QString &name, Joint *parent = 0);
    ~Joint();

//    Joint *clone() const;

    Joint *parent() const;
    Joint *child(int i) const;
    QList<Joint *> children() const;

    void addChild(Joint *child);
    void insertChild(int index, Joint *child);
    void removeChild(Joint *child);
    Joint *takeChild(int i);

//    void addChildren(const QList<Joint*> &children);
//    void insertChildren(int index, const QList<Joint*> &children);
//    QList<Joint*> takeChildren();

    QString name() const;
    void setName(const QString &name);

    bool isVisible() const;
    void setVisible(bool visible);

    QMap<Anim *, KeyFrames *> anims() const;

private:
    friend class JointModel;

    void syncAnims(Joint *joint) const;

    JointModel *m_model;

    Joint *m_parent;
    QList<Joint*> m_children;

    QString m_name;
    bool m_isVisible;
    QMap<Anim *, KeyFrames *> m_anims;
};

#endif // JOINT_H
