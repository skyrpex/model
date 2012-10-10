#ifndef KEYFRAMES_H
#define KEYFRAMES_H

#include <QObject>
#include <QMap>
#include <QMetaType>

class Transform;

typedef QMap<int, Transform*> KeyFrameMap;

class KeyFrames : public QObject
{
    Q_OBJECT
public:
    explicit KeyFrames(QObject *parent = 0);
    ~KeyFrames();

    KeyFrameMap *data;
};

Q_DECLARE_METATYPE(KeyFrames*)

#endif // KEYFRAMES_H
