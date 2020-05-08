#include "Matrix.h"

#include "Vector.h"

#include <cmath>

#define arc(x) (float)(x / 180 * 3.141592653589793)

namespace LLWP
{
    Matrix::Matrix() :
        mat_{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }
    {
    }
    Matrix::Matrix(const Matrix& m) :
        mat_(m.mat_)
    {
    }
    Matrix::Matrix(const D2D1_MATRIX_4X4_F& m) :
        mat_(m)
    {
    }
    Matrix::Matrix(const Vector& scl, float rot, const Vector& pos):
        mat_
    {
         scl.x() * cos(arc(rot)), scl.x()* sin(arc(rot)),  0,  0,
        -scl.y() * sin(arc(rot)), scl.y()* cos(arc(rot)),  0,  0,
                               0,                      0,  1,  0,
                         pos.x(),                pos.y(),  0,  1
    }
    {
    }
    Matrix& Matrix::operator=(const Matrix& m)
    {
        mat_ = m.mat_;
        return *this;
    }
    Matrix& Matrix::operator=(Matrix&& m)
    {
        mat_ = m.mat_;
        return *this;
    }
    Matrix Matrix::operator*(const Matrix& m)
    {
        return Matrix
        ({
            mat_.m[0][0] * m(0, 0) + mat_.m[0][1] * m(1, 0) + mat_.m[0][2] * m(2, 0) + mat_.m[0][3] * m(3, 0),
            mat_.m[0][0] * m(0, 1) + mat_.m[0][1] * m(1, 1) + mat_.m[0][2] * m(2, 1) + mat_.m[0][3] * m(3, 1),
            mat_.m[0][0] * m(0, 2) + mat_.m[0][1] * m(1, 2) + mat_.m[0][2] * m(2, 2) + mat_.m[0][3] * m(3, 2),
            mat_.m[0][0] * m(0, 3) + mat_.m[0][1] * m(1, 3) + mat_.m[0][2] * m(2, 3) + mat_.m[0][3] * m(3, 3),

            mat_.m[1][0] * m(0, 0) + mat_.m[1][1] * m(1, 0) + mat_.m[1][2] * m(2, 0) + mat_.m[1][3] * m(3, 0),
            mat_.m[1][0] * m(0, 1) + mat_.m[1][1] * m(1, 1) + mat_.m[1][2] * m(2, 1) + mat_.m[1][3] * m(3, 1),
            mat_.m[1][0] * m(0, 2) + mat_.m[1][1] * m(1, 2) + mat_.m[1][2] * m(2, 2) + mat_.m[1][3] * m(3, 2),
            mat_.m[1][0] * m(0, 3) + mat_.m[1][1] * m(1, 3) + mat_.m[1][2] * m(2, 3) + mat_.m[1][3] * m(3, 3),
            
            mat_.m[2][0] * m(0, 0) + mat_.m[2][1] * m(1, 0) + mat_.m[2][2] * m(2, 0) + mat_.m[2][3] * m(3, 0),
            mat_.m[2][0] * m(0, 1) + mat_.m[2][1] * m(1, 1) + mat_.m[2][2] * m(2, 1) + mat_.m[2][3] * m(3, 1),
            mat_.m[2][0] * m(0, 2) + mat_.m[2][1] * m(1, 2) + mat_.m[2][2] * m(2, 2) + mat_.m[2][3] * m(3, 2),
            mat_.m[2][0] * m(0, 3) + mat_.m[2][1] * m(1, 3) + mat_.m[2][2] * m(2, 3) + mat_.m[2][3] * m(3, 3),

            mat_.m[3][0] * m(0, 0) + mat_.m[3][1] * m(1, 0) + mat_.m[3][2] * m(2, 0) + mat_.m[3][3] * m(3, 0),
            mat_.m[3][0] * m(0, 1) + mat_.m[3][1] * m(1, 1) + mat_.m[3][2] * m(2, 1) + mat_.m[3][3] * m(3, 1),
            mat_.m[3][0] * m(0, 2) + mat_.m[3][1] * m(1, 2) + mat_.m[3][2] * m(2, 2) + mat_.m[3][3] * m(3, 2),
            mat_.m[3][0] * m(0, 3) + mat_.m[3][1] * m(1, 3) + mat_.m[3][2] * m(2, 3) + mat_.m[3][3] * m(3, 3),
        });
    }
    Matrix::operator D2D1_MATRIX_4X4_F()
    {
        return mat_;
    }
    float Matrix::operator()(int i, int j) const
    {
        return mat_.m[i][j];
    }

}
