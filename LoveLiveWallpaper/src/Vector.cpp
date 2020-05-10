#include "Vector.h"
#include "Matrix.h"

using namespace ::DirectX;

namespace LLWP
{
    Vector::Vector(float a, float b, float c):
        vec{ a, b, c, 1 }
    {
    }

    Vector::Vector(DirectX::FXMVECTOR& v) : vec(v)
    {
    }

    float Vector::x() const
    {
        return DirectX::XMVectorGetX(vec);
    }

    float Vector::y() const
    {
        return DirectX::XMVectorGetY(vec);
    }

    float Vector::z() const
    {
        return DirectX::XMVectorGetZ(vec);
    }

    Vector::operator DirectX::XMVECTOR()
    {
        return vec;
    }

    Vector Vector::operator+(const Vector& right)
    {
        return XMVectorAdd(vec, right.vec);
    }

    Vector& Vector::operator+=(const Vector& d)
	{
        vec = XMVectorAdd(vec, d.vec);
        return *this;
	}

    Vector& Vector::operator*=(float s)
    {
        vec = XMVectorScale(vec, s);
        return *this;
    }

    Vector& Vector::operator*=(const Matrix& m)
    {
        vec = XMVector3Transform(vec, m);
        return *this;
    }

    Vector Vector::operator*(const Matrix& m)
    {
        return DirectX::XMVector3Transform(*this, m);
    }

	Vector Vector::operator-()
	{
        return XMVectorScale(vec, -1);
	}

    Vector::~Vector()
    {
    }

    Vector operator/(float f, Vector vec)
    {
        return Vector{ f / vec.x(), f / vec.y(), f / vec.z() };
    }

    Vector operator/(Vector vec, float f)
    {
        return XMVectorScale(vec, 1.f / f);
    }
}
