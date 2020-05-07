#include "GameObject.h"

namespace LLWP
{
    Transform::Transform(GameObject& o):
        obj(o),
        parent_(nullptr),
        size_{ 100,100,0 },
        position_{ 0,0,0 },
        scale_{ 1,1,1 },
        rotation_(0),
        localToWorldMatrix(scale_, rotation_, position_),
        worldToLocalMatrix(1/scale_, -rotation_, -position_),
        localPosition_{ 0,0 },
        localScale_{ 1,1 },
        locaRotation(0),
        localToParentMatrix(localScale_, locaRotation, localPosition_)
    {
    }
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
    void Transform::setParent(Transform& p)
    {
        parent_ = &p;
    }

    Transform* Transform::parent()
    {
        return parent_;
    }

    bool Transform::HitTest(long x, long y)
    {
        Vector tmp = Vector{ (float)x, (float)y, 0 } * worldToLocalMatrix;
        return (tmp.x() > 0 &&
                tmp.x() < size_.x() &&
                tmp.y() > 0 &&
                tmp.y() < size_.y());
    }

    Vector& Transform::Size()
    {
        return size_;
    }

    const Vector& Transform::Size() const
    {
        return size_;
    }

    const Vector& Transform::Position() const
    {
        return position_;
    }

    D2D1_RECT_F Transform::rect()
    {
        return D2D1_RECT_F{ 0,0,size_.x(), size_.y() };
    }
     
    D2D1_MATRIX_4X4_F Transform::localToWorld()
    {
        return localToWorldMatrix;
    }

    D2D1_MATRIX_4X4_F Transform::worldToLocal()
    {
        return worldToLocalMatrix;
    }

    void Transform::Move(float x, float y)
    {
        position_ += {x, y};
        localToWorldMatrix = Matrix(scale_, rotation_, position_);
        worldToLocalMatrix = Matrix(1/scale_, -rotation_, -position_);
    }

    void Transform::Rotate(float angle)
    {
        rotation_ += angle;
        for (; rotation_ >= 360; rotation_ -= 360) {}
        for (; rotation_ < 0; rotation_ += 360) {}
        localToWorldMatrix =
            Matrix({ 1,1 }, 0, -(size_ / 2)) *
            Matrix(scale_, rotation_, { 0,0 }) *
            Matrix({ 1,1 }, 0, position_ + (size_ / 2));
        worldToLocalMatrix = Matrix(1 / scale_, -rotation_, -position_);
    }

    Transform::~Transform()
    {
    }
}