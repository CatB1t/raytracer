#pragma once

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

    Point3D<T> operator+(const Point3D& other)
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    Point3D<T> operator-(const Point3D& other)
    {
        return {x - other.x, y - other.y, z - other.z};
    }
};
