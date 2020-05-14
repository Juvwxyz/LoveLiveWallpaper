#pragma once
#include <DirectXMath.h>

namespace LLWP
{
    class Matrix;

    class Vector
    {
    public :
        Vector(float a = 0, float b = 0, float c = 0);
        Vector(DirectX::FXMVECTOR& m);

        float x() const;
        float y() const;
        float z() const;

        operator DirectX::XMVECTOR() const;

        Vector operator+(const Vector& right);
        Vector& operator+=(const Vector& d);

        Vector operator*(const Matrix& m);
        Vector operator*(float f);
        Vector operator*(const Vector& right);
        Vector& operator*=(float s);
        Vector& operator*=(const Matrix& m);

        Vector operator-();

        ~Vector();

    private:
        DirectX::XMVECTOR vec;
    };

    Vector operator/(float f, Vector vec);
    Vector operator/(Vector vec, float f);
}