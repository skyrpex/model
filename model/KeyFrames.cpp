#include "KeyFrames.h"
#include "Transform.h"

KeyFrames::KeyFrames(QObject *parent) :
    QObject(parent),
    data(new KeyFrameMap)
{
    data->insert(0, new Transform);
}

KeyFrames::~KeyFrames()
{
    delete data;
}
