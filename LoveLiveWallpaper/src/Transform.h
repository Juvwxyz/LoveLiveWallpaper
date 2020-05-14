#pragma once
#include "Matrix.h"
#include "Vector.h"
#include "typedef.h"

#include <d2d1_1.h>
#include <list>
#include <memory>

namespace LLWP
{
    class GameObject;

    class Transform : public ::std::enable_shared_from_this<Transform>
    {
    public :
        Transform(GameObject&);
        Transform(const Transform&);
        Transform(const Transform&&);
        Transform& operator=(const Transform&);
        Transform& operator=(const Transform&&);

        void SetParent(Transform& p);
        void SetParent(::std::shared_ptr<Transform> p);
        void SetParent(GameObject& obj);
        void SetParent(::std::shared_ptr<GameObject> obj);
        ::std::shared_ptr<Transform> parent();

        bool HitTest(long x, long y) const;

        Vector& size();
        const Vector& size() const;

        const Vector& position() const;
        Vector& position();

        const Vector& scale() const;
        Vector& scale();

        float rotation() const;
        float& rotation();

        Matrix localToWorld() const;
        Matrix worldToLocal() const;

        void Move(float x, float y);
        void ScaleBy(float s);
        void Rotate(float angle);

        ~Transform();

    private:
        GameObject& obj;
        ::std::shared_ptr<Transform> _parent;
        ::std::list<::std::shared_ptr<Transform>> _children;

        Vector _size;

        Vector _position;
        Vector _scale;
        float _rotation;
        Matrix _localToWorldMatrix;

        Matrix _worldToLocalMatrix;

        Vector _localPosition;
        Vector _localScale;
        float _localRotation;
        Matrix _localToParentMatrix;
    };
}