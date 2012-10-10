#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QWidget>
#include <QModelIndex>

class JointModel;
class JointDelegate;
class JointProxyModel;
class QTreeView;

class TimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimeWidget(JointModel *model, QWidget *parent = 0);

public slots:
    void setCurrentAnim(int i);

private slots:
    void openEditor(const QModelIndex &index);
    void onExpanded(const QModelIndex &proxyIndex);
    void onCollapsed(const QModelIndex &proxyIndex);
    void resetEditor();

private:
    JointDelegate *m_delegate;
    JointProxyModel *m_leftProxy;
    JointProxyModel *m_rightProxy;
    QTreeView *m_leftView;
    QTreeView *m_rightView;
    QModelIndex m_openEditorIndex;
};

#endif // TIMEWIDGET_H
