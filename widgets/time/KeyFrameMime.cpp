#include "KeyFrameMime.h"
#include "KeyFrameItem.h"

KeyFrameMime::KeyFrameMime(KeyFrameItem *keyFrameItem) :
    m_keyFrameItem(keyFrameItem)
{
}

KeyFrameItem *KeyFrameMime::keyFrameItem() const
{
    return m_keyFrameItem;
}
