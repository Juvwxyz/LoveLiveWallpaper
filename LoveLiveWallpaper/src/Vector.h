#pragma once


namespace LLWP
{
    class Matrix;

    class Vector
    {
    public :
        Vector(float a = 0, float b = 0, float c = 0);

        float& x();
        float& y();
        float& z();

        float x() const;
        float y() const;
        float z() const;

        void operator+=(const Vector& d);

        Vector& operator*=(const Matrix& m);
        Vector operator*(const Matrix& m);

        Vector operator-();

        ~Vector();
    private:
        float x_;
        float y_;
        float z_;
    };

    inline Vector operator/(float f, Vector vec)
    {
        return Vector{ f / vec.x(), f / vec.y(), f / vec.z() };
    }
}