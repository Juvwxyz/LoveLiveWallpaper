#pragma once
#include "typedef.h"

#include <d2d1_1.h>

namespace LLWP
{
    class Vector;

    class Matrix
    {
    public:
        Matrix();
        Matrix(const Matrix& m);
        Matrix(const D2D1_MATRIX_4X4_F &m);
        Matrix(const Vector& scl, float rot, const Vector& pos);
        Matrix& operator=(const Matrix& m);
        Matrix& operator=(Matrix&& m);

        Matrix operator*(const Matrix& m);
        operator D2D1_MATRIX_4X4_F();

        float operator()(int i, int j) const;
    private:

        D2D1_MATRIX_4X4_F mat_;
        
    };

}