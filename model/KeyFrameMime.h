#ifndef KEYFRAMEMIME_H
#define KEYFRAMEMIME_H

#include <QMimeData>

class KeyFrameItem;

class KeyFrameMime : public QMimeData
{
    Q_OBJECT
public:
    explicit KeyFrameMime(KeyFrameItem *keyFrameItem);

    KeyFrameItem *keyFrameItem() const;

private:
    KeyFrameItem *m_keyFrameItem;
};

#endif // KEYFRAMEMIME_H
