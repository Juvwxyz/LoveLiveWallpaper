#pragma once
#include "Matrix.h"
#include "Vector.h"
#include "typedef.h"

#include <d2d1_1.h>

namespace LLWP
{
    class GameObject;

    class Transform
    {
    public :
        Transform(GameObject&);
        Transform(const Transform&);
        Transform(const Transform&&);
        Transform& operator=(const Transform&);
        Transform& operator=(const Transform&&);

        void setParent(Transform& p);
        Transform* parent();

        bool HitTest(long x, long y);

        Vector& Size();
        const Vector& Size() const;

        const Vector& Position() const;
        const Vector& Scale() const;
        float Rotation() const;

        D2D1_RECT_F rect();
        Matrix localToWorld();
        Matrix worldToLocal();

        //void setPosition(const Vector& v);
        //Vector getPosition();
        void Move(float x, float y);
        void ScaleBy(float s);
        void Rotate(float angle);

        operator DirectX::XMMATRIX();

        ~Transform();

    private:
        GameObject& obj;
        Transform* parent_;

        Vector size_;

        Vector position_;
        Vector scale_;
        float rotation_;
        Matrix localToWorldMatrix;

        Matrix worldToLocalMatrix;

        Vector localPosition_;
        Vector localScale_;
        float locaRotation;
        Matrix localToParentMatrix;
    };
}