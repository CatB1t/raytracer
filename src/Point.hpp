#pragma once

#include <cassert>
#include <cmath>

template<typename T>
struct Point2D
{
    T x;
    T y;
};

template<typename T>
struct Point3D
{
    T x = 0;
    T y = 0;
    T z = 0;

    T dot(const Point3D<T>& other)
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Point3D<T> normalize() const
    {
        T sqr_sum = this->length();
        assert(std::abs(sqr_sum) > 0);
        return {x / sqr_sum, y / sqr_sum, z / sqr_sum};
    }

    T length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    Point3D<T> operator*(float val) const
    {
        return {x * val, y * val, z * val};
    }

    Point3D<T> operator+(const Point3D& other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    Point3D<T> operator-(const Point3D& other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }
};
