#include "Transform.h"

Transform::Transform() :
    rotation(0),
    length(100)
{
}

Transform::Transform(qreal rotation, qreal length) :
    rotation(rotation),
    length(length)
{
}

Transform::Transform(const Transform &other) :
    rotation(other.rotation),
    length(other.length)
{
}

void Transform::operator =(const Transform &b)
{
    rotation = b.rotation;
    length = b.length;
}
