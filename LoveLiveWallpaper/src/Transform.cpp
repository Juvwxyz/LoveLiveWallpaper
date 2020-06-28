#include "Transform.h"

#include "Application.h"
#include "GameObject.h"
#include "Graphics.h"

namespace LLWP
{
    Transform::Transform(GameObject& o):
        obj(o),
        _parent(nullptr),
        _size{ 100,100,0 },
        _position{ 0,0,0 },
        _scale{ 1,1,1 },
        _rotation(0),
        _localToWorldMatrix(_scale, _rotation, _position),
        _worldToLocalMatrix(1/_scale, -_rotation, -_position),
        _localPosition{ 0,0 },
        _localScale{ 1,1,1 },
        _localRotation(0),
        _localToParentMatrix(_localScale, _localRotation, _localPosition)
    {}

    Transform::Transform(const Transform& t):
        obj(t.obj)
    {
    }
    Transform::Transform(const Transform&& t) :
        obj(t.obj)
    {
    }
    Transform& Transform::operator=(const Transform&)
    {
        return *this;
    }
    Transform& Transform::operator=(const Transform&&)
    {
        return *this;
    }
    void Transform::SetParent(Transform& p)
    {
        _parent = p.shared_from_this();
    }

    void Transform::SetParent(::std::shared_ptr<Transform> p)
    {
        _parent = p;
    }

    void Transform::SetParent(GameObject& obj)
    {
        _parent = obj.transform().shared_from_this();
    }

    void Transform::SetParent(::std::shared_ptr<GameObject> obj)
    {
        _parent = obj->transform().shared_from_this();
    }

    ::std::shared_ptr<Transform> Transform::parent()
    {
        return _parent;
    }

    bool Transform::HitTest(long x, long y) const
    {
        x /= Graphics::getScaleFactor();
        y /= Graphics::getScaleFactor();
        Vector tmp = Vector{ (float)x-960, 540-(float)y, 0 } * _worldToLocalMatrix;
        return (tmp.x() > -_size.x()/2 &&
                tmp.x() < _size.x()/2 &&
                tmp.y() > -_size.y()/2 &&
                tmp.y() < _size.y()/2);
    }

    Vector& Transform::size()
    {
        return _size;
    }

    const Vector& Transform::size() const
    {
        return _size;
    }

    const Vector& Transform::position() const
    {
        return _position;
    }

    Vector& Transform::position()
    {
        return _position;
    }

    const Vector& Transform::scale() const
    {
        return _scale;
    }

    Vector& Transform::scale()
    {
        return _scale;
    }

    float Transform::rotation() const
    {
        return _rotation;
    }

    float& Transform::rotation()
    {
        return _rotation;
    }

    Matrix Transform::localToWorld() const
    {
        return _localToWorldMatrix;
    }

    Matrix Transform::worldToLocal() const
    {
        return _worldToLocalMatrix;
    }

    void Transform::Move(float x, float y)
    {
        _position += {x, y};
        _localToWorldMatrix = Matrix(_scale, _rotation, _position);
        _worldToLocalMatrix = Matrix(1 / _scale, -_rotation, -_position * (1 / _scale));
    }

    void Transform::ScaleBy(float s)
    {
        _scale = Vector(_scale.x() * s, _scale.y() * s, 1);
        _size = Vector(_size.x() * s, _size.y() * s, _size.z());
        _localToWorldMatrix = Matrix(_scale, _rotation, _position);
        _worldToLocalMatrix = Matrix(1 / _scale, -_rotation, -_position * (1 / _scale));
    }

    void Transform::Rotate(float angle)
    {
        _rotation += angle;
        for (; _rotation >= 360; _rotation -= 360) {}
        for (; _rotation < 0; _rotation += 360) {}
        _localToWorldMatrix = Matrix(_scale, _rotation, _position);
        _worldToLocalMatrix = Matrix(1 / _scale, -_rotation, -_position * (1 / _scale));
    }

    Transform::~Transform()
    {
    }
}