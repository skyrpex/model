#include "TimeWidget.h"
#include "model/JointModel.h"
#include "model/AnimModel.h"
#include "widgets/time/JointProxyModel.h"
#include "widgets/time/JointDelegate.h"
#include "widgets/time/JointHeaderView.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QSplitter>
#include <QScrollbar>

TimeWidget::TimeWidget(JointModel *model, QWidget *parent) :
    QWidget(parent),
    m_delegate(new JointDelegate(this)),
    m_leftProxy(new JointProxyModel(model, this)),
    m_rightProxy(new JointProxyModel(model, this)),
    m_leftView(new QTreeView(this)),
    m_rightView(new QTreeView(this))
{
    QSplitter *splitter = new QSplitter(this);
    connect(m_leftView->verticalScrollBar(), SIGNAL(valueChanged(int)), m_rightView->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_rightView->verticalScrollBar(), SIGNAL(valueChanged(int)), m_leftView->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_leftView, SIGNAL(expanded(QModelIndex)), SLOT(onExpanded(QModelIndex)));
    connect(m_leftView, SIGNAL(collapsed(QModelIndex)), SLOT(onCollapsed(QModelIndex)));

    {
        m_leftProxy->showAnims(false);

        m_leftView->setModel(m_leftProxy);
        m_leftView->setItemDelegate(m_delegate);
        m_leftView->setHeader(new JointHeaderView(false, this));
        m_leftView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_leftView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        splitter->addWidget(m_leftView);
    }

    {
        JointHeaderView *header = new JointHeaderView(true, this);

        m_rightProxy->showVisibleColumn(false);
        m_rightProxy->showAnim(0);

        m_rightView->setModel(m_rightProxy);
        m_rightView->setItemDelegate(m_delegate);
        m_rightView->setHeader(header);
        m_rightView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        m_rightView->hideColumn(JointModel::NameColumn);
        m_rightView->setAutoScroll(false);
        m_rightView->setMouseTracking(true);
        m_rightView->setItemsExpandable(false);

        connect(m_rightView, SIGNAL(entered(QModelIndex)), SLOT(openEditor(QModelIndex)));
        connect(model->animModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(resetEditor()));
        connect(model->animModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(resetEditor()));
        connect(model->animModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(resetEditor()));

        connect(m_delegate, SIGNAL(currentFrameChanged(int)), header, SLOT(setCurrentFrame(int)));
        connect(header, SIGNAL(currentFrameChanged(int)), m_delegate, SLOT(setCurrentFrame(int)));
        connect(header, SIGNAL(currentFrameChanged(int)), m_rightView->viewport(), SLOT(update()));

        splitter->addWidget(m_rightView);
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(splitter);
}

void TimeWidget::setCurrentAnim(int i)
{
    m_rightProxy->showAnim(i);
}

void TimeWidget::openEditor(const QModelIndex &index)
{
    if(m_openEditorIndex.isValid())
    {
        m_rightView->closePersistentEditor(m_openEditorIndex);
        m_openEditorIndex = QModelIndex();
    }

    if(index.column() != JointModel::NameColumn)
    {
        m_rightView->openPersistentEditor(index);
        m_openEditorIndex = index;
    }
}

void TimeWidget::onExpanded(const QModelIndex &proxyIndex)
{
    QModelIndex sourceIndex = m_leftProxy->mapToSource(proxyIndex);
    m_rightView->expand(m_rightProxy->mapFromSource(sourceIndex));
}

void TimeWidget::onCollapsed(const QModelIndex &proxyIndex)
{
    QModelIndex sourceIndex = m_leftProxy->mapToSource(proxyIndex);
    m_rightView->collapse(m_rightProxy->mapFromSource(sourceIndex));
}

void TimeWidget::resetEditor()
{
    openEditor(m_rightView->currentIndex());
}
