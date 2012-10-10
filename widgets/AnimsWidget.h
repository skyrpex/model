#ifndef ANIMSWIDGET_H
#define ANIMSWIDGET_H

#include <QWidget>

class AnimModel;
class QTreeView;
class QModelIndex;
class QAction;

class AnimsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnimsWidget(AnimModel *model, QWidget *parent = 0);

signals:
    void currentAnimChanged(int i);
    
private slots:
    void createAnim();
    void editAnim();
    void removeAnim();
    void updateActions();
    void onCurrentRowChanged(const QModelIndex &index);

private:
    AnimModel *m_model;
    QTreeView *m_view;
    QAction *m_createAction;
    QAction *m_editAction;
    QAction *m_removeAction;
};

#endif // ANIMSWIDGET_H
