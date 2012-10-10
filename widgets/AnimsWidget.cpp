#include "AnimsWidget.h"
#include "model/AnimModel.h"
#include "dialogs/AnimDialog.h"
#include "model/Anim.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>

AnimsWidget::AnimsWidget(AnimModel *model, QWidget *parent) :
    QWidget(parent),
    m_model(model),
    m_view(new QTreeView(this))
{
    m_view->setModel(model);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    {
        QToolBar *t = new QToolBar(this);
        m_createAction = t->addAction("Create Anim", this, SLOT(createAnim()));
        m_createAction->setShortcut(QKeySequence("Ctrl+A"));
        m_editAction = t->addAction("Edit Anim", this, SLOT(editAnim()));
        m_editAction->setShortcut(QKeySequence("Ctrl+E"));
        m_removeAction = t->addAction("Remove Anim", this, SLOT(removeAnim()));
        m_removeAction->setShortcut(QKeySequence("Ctrl+R"));
        layout->addWidget(t);
    }
    layout->addWidget(m_view);

    connect(m_view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(onCurrentRowChanged(QModelIndex)));
    connect(m_view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(updateActions()));

    updateActions();
}

void AnimsWidget::onCurrentRowChanged(const QModelIndex &index)
{
    emit currentAnimChanged(index.row());
}

void AnimsWidget::createAnim()
{
    AnimDialog d(this);
    if(!d.exec()) return;

    m_model->addAnim(new Anim(d.name(), d.frameCount(), d.fps()));

    // Set the new anim as current index
    m_view->setCurrentIndex(m_model->index(m_model->anims().count()-1, 0, QModelIndex()));
}

void AnimsWidget::editAnim()
{
    Anim *anim = static_cast<Anim *>(m_view->currentIndex().internalPointer());

    AnimDialog d(anim->name(), anim->frameCount(), anim->fps(), this);
    if(!d.exec()) return;

    anim->setName(d.name());
    anim->setFrameCount(d.frameCount());
    anim->setFps(d.fps());
}

void AnimsWidget::removeAnim()
{
    delete static_cast<Anim *>(m_view->currentIndex().internalPointer());
    emit currentAnimChanged(m_view->currentIndex().row());
}

void AnimsWidget::updateActions()
{
    bool hasCurrent = m_view->currentIndex().isValid();
    m_editAction->setEnabled(hasCurrent);
    m_removeAction->setEnabled(hasCurrent);
}
