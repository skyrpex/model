#include "Anim.h"
#include "AnimModel.h"
#include <QDebug>

Anim::Anim(const QString &name, int frameCount, int fps) :
    m_model(NULL),
    m_name(name),
    m_frameCount(frameCount),
    m_fps(fps)
{
}

Anim::~Anim()
{
    if(m_model)
        m_model->removeAnim(this);
}

QString Anim::name() const
{
    return m_name;
}

void Anim::setName(const QString &name)
{
    if(m_name == name)
        return;

    m_name = name;
    if(m_model)
        m_model->emitAnimChanged(this, AnimModel::NameColumn);
}

int Anim::frameCount() const
{
    return m_frameCount;
}

void Anim::setFrameCount(int frameCount)
{
    if(m_frameCount == frameCount)
        return;

    m_frameCount = frameCount;
    if(m_model)
        m_model->emitAnimChanged(this, AnimModel::FrameCountColumn);
}

int Anim::fps() const
{
    return m_fps;
}

void Anim::setFps(int fps)
{
    if(m_fps == fps)
        return;

    m_fps = fps;
    if(m_model)
        m_model->emitAnimChanged(this, AnimModel::FpsColumn);
}
