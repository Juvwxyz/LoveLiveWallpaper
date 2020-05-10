#pragma once
#include "typedef.h"

#include <d2d1_1.h>
#include <DirectXMath.h>

namespace LLWP
{
    class Vector;

    class Matrix : public DirectX::XMMATRIX
    {
    public:
        Matrix();
        Matrix(const Matrix& m) = default;
        Matrix(float m00, float m01, float m02, float m03,
               float m10, float m11, float m12, float m13,
               float m20, float m21, float m22, float m23,
               float m30, float m31, float m32, float m33);
        Matrix(const DirectX::XMMATRIX& m);
        Matrix(const D2D1_MATRIX_4X4_F &m);
        Matrix(const Vector& scl, float rot, const Vector& pos);
        Matrix& operator=(const Matrix& m) = default;
        Matrix& operator=(Matrix&& m) = default;

        Matrix operator*(const Matrix& m);
        operator D2D1_MATRIX_4X4_F();

        float operator()(int i, int j) const;

    };

}