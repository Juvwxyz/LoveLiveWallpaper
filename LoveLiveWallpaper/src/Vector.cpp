#include "Vector.h"
#include "Matrix.h"

namespace LLWP
{
    Vector::Vector(float a, float b, float c):
        x_(a), y_(b), z_(c)
    {
    }

    float& Vector::x()
    {
        return x_;
    }
    float& Vector::y()
    {
        return y_;
    }
    float& Vector::z()
    {
        return z_;
    }

    float Vector::x() const
    {
        return x_;
    }
    float Vector::y() const
    {
        return y_;
    }
    float Vector::z() const
    {
        return z_;
    }

	void Vector::operator+=(const Vector& d)
	{
        x_ += d.x_;
        y_ += d.y_;
        z_ += d.z_;
	}

    Vector& Vector::operator*=(const Matrix& m)
    {
        float tx = x_ * m(0, 0) + y_ * m(1, 0) + z_ * m(2, 0) + m(3, 0);
        float ty = x_ * m(0, 1) + y_ * m(1, 1) + z_ * m(2, 1) + m(3, 1);
        float tz = x_ * m(0, 2) + y_ * m(1, 2) + z_ * m(2, 2) + m(3, 2);
        x_ = tx;
        y_ = ty;
        z_ = tz;
        return *this;
    }

    Vector Vector::operator*(const Matrix& m)
    {
        return Vector
        {
            x_* m(0, 0) + y_ * m(1, 0) + z_ * m(2, 0) + m(3, 0),
            x_* m(0, 1) + y_ * m(1, 1) + z_ * m(2, 1) + m(3, 1),
            x_* m(0, 2) + y_ * m(1, 2) + z_ * m(2, 2) + m(3, 2)
        };
    }

	Vector Vector::operator-()
	{
        return Vector(-x_, -y_, -z_);
	}

    Vector::~Vector()
    {
    }

    Vector operator/(float f, Vector vec)
    {
        return Vector{ f / vec.x(), f / vec.y(), f / vec.z() };
    }
}
