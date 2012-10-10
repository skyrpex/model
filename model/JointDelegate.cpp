#include "JointDelegate.h"
#include "Anim.h"
#include "JointModel.h"
#include "KeyFrames.h"
#include "KeyFramesEditor.h"
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QPainter>

static const QSize MinimumSizeHint(0, 20);
static const QPen LinePeN(QColor(250, 0, 0), 1);
static const int LineOffset = 3;

JointDelegate::JointDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    m_currentFrame(0)
{
}

QWidget *JointDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data().canConvert<KeyFrames *>())
    {
        KeyFramesEditor *editor = new KeyFramesEditor(parent);
        connect(editor, SIGNAL(currentFrameChanged(int)), SLOT(setCurrentFrame(int)));
        connect(this, SIGNAL(currentFrameChanged(int)), editor, SLOT(setCurrentFrame(int)));
        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void JointDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.data().canConvert<KeyFrames *>())
    {
        Anim *anim = index.data(JointModel::AnimRole).value<Anim *>();
        KeyFrames *keyFrames = index.data().value<KeyFrames *>();
        KeyFramesEditor *keyFramesEditor = qobject_cast<KeyFramesEditor *>(editor);
        Q_ASSERT(anim);
        Q_ASSERT(keyFramesEditor);
        Q_ASSERT(keyFrames);
        keyFramesEditor->setData(keyFrames->data, anim->frameCount(), m_currentFrame);
    }
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void JointDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data().canConvert<KeyFrames *>())
        paintAnim(painter, option, index.data(JointModel::AnimRole).value<Anim *>(), index.data().value<KeyFrames *>());
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize JointDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size(8, 20) /*= AnimEditor::frameSize()*/;
    if(index.data().canConvert<KeyFrames *>())
    {
        Anim *anim = index.data(JointModel::AnimRole).value<Anim *>();
        if(anim)
            size.rwidth() *= anim->frameCount();
    }

    return QStyledItemDelegate::sizeHint(option, index).expandedTo(size);
}

void JointDelegate::paintAnim(QPainter *painter, const QStyleOptionViewItem &option, Anim *anim, KeyFrames *keyFrames) const
{
    Q_ASSERT(anim);
    Q_ASSERT(keyFrames);

    // Draw the background
    painter->save();
    painter->translate(option.rect.topLeft());
    painter->fillRect(QRectF(QPointF(), option.rect.size()), QPixmap(":/images/background"));

    QPixmap pixmap(":/images/keyframe");

    // Draw the keyframes
    if(anim)
    {
        foreach(int frame, keyFrames->data->keys())
        {
            int x = frame*pixmap.width();
            QRectF rect(QPointF(x, 0), pixmap.size());
            painter->fillRect(rect, pixmap);
        }
    }

    // Draw time marker
    int x = LineOffset + m_currentFrame*pixmap.width();
    painter->setPen(LinePeN);
    painter->drawLine(x, 0, x, pixmap.height()-1);
    painter->restore();

    // Disabled & offseted frames
    painter->save();
    painter->setCompositionMode(QPainter::CompositionMode_Multiply);

    // Ofsetted frames are darker
    QBrush brush(option.palette.base().color().darker(120));
    int xOffset = (anim? anim->frameCount() : 0)*pixmap.width();
    QRectF rect = option.rect.adjusted(xOffset, 0, 0, 0);
    painter->fillRect(rect, brush);

    // Gray out if disabled
    painter->fillRect(option.rect, option.palette.base());
    painter->restore();
}

void JointDelegate::setCurrentFrame(int frame)
{
    if(m_currentFrame != frame)
    {
        m_currentFrame = frame;
        emit currentFrameChanged(frame);
    }
}
