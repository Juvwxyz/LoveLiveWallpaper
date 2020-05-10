#include "Matrix.h"

#include "Exception.h"
#include "Vector.h"

#include <cmath>
#include <exception>

#define arc(x) (float)(x / 180 * 3.141592653589793)

using namespace DirectX;

namespace LLWP
{
    Matrix::Matrix() : Matrix{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }
    {}

    Matrix::Matrix(float m00, float m01, float m02, float m03,
                   float m10, float m11, float m12, float m13,
                   float m20, float m21, float m22, float m23,
                   float m30, float m31, float m32, float m33) :
        XMMATRIX(m00, m01, m02, m03,
                 m10, m11, m12, m13,
                 m20, m21, m22, m23,
                 m30, m31, m32, m33)
    {
    }

    Matrix::Matrix(const XMMATRIX& m) : XMMATRIX(m)
    {
    }

    Matrix::Matrix(const D2D1_MATRIX_4X4_F& m) :
        Matrix{
            m.m[0][0],m.m[0][1],m.m[0][2],m.m[0][3],
            m.m[1][0],m.m[1][1],m.m[1][2],m.m[1][3],
            m.m[2][0],m.m[2][1],m.m[2][2],m.m[2][3],
            m.m[3][0],m.m[3][1],m.m[3][2],m.m[3][3]
        }
    {
    }
    Matrix::Matrix(const Vector& scl, float rot, const Vector& pos):
        Matrix{
             scl.x() * cos(arc(rot)), scl.x() * sin(arc(rot)),  0,  0,
            -scl.y() * sin(arc(rot)), scl.y() * cos(arc(rot)),  0,  0,
                                   0,                       0,  1,  0,
                             pos.x(),                 pos.y(),  0,  1
        }
    {}

    Matrix Matrix::operator*(const Matrix& m)
    {
        return XMMatrixMultiply(*this, m);
    }

    Matrix::operator D2D1_MATRIX_4X4_F()
    {
        return D2D1_MATRIX_4X4_F{
            XMVectorGetX(r[0]),XMVectorGetY(r[0]),XMVectorGetZ(r[0]),XMVectorGetW(r[0]),
            XMVectorGetX(r[1]),XMVectorGetY(r[1]),XMVectorGetZ(r[1]),XMVectorGetW(r[1]),
            XMVectorGetX(r[2]),XMVectorGetY(r[2]),XMVectorGetZ(r[2]),XMVectorGetW(r[2]),
            XMVectorGetX(r[3]),XMVectorGetY(r[3]),XMVectorGetZ(r[3]),XMVectorGetW(r[3]),
        };
    }
    float Matrix::operator()(int i, int j) const
    {
        switch (j)
        {
        case 0: return XMVectorGetX(r[i]);
        case 1: return XMVectorGetY(r[i]);
        case 2: return XMVectorGetZ(r[i]);
        case 3: return XMVectorGetW(r[i]);
        }
        throw(IndexOutofRangeException());
    }

}
