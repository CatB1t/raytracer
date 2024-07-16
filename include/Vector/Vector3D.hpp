#pragma once

#include <cassert>
#include <cmath>

template <typename T> struct Vector3D {
  T x = 0;
  T y = 0;
  T z = 0;

  T dot(const Vector3D<T> &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  Vector3D<T> normalize() const {
    T sqr_sum = this->length();
    assert(std::abs(sqr_sum) > 0);
    return {x / sqr_sum, y / sqr_sum, z / sqr_sum};
  }

  Vector3D<T> reflect(const Vector3D<T>& normal) {
    return 2 * normal * normal.dot(*this) - *this;
  }

  T length() const { return sqrt(x * x + y * y + z * z); }

  Vector3D<T> operator*(float val) const { return {x * val, y * val, z * val}; }

  Vector3D<T> operator+(const Vector3D &other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  Vector3D<T> operator-(const Vector3D &other) const {
    return {x - other.x, y - other.y, z - other.z};
  }
};

template <typename T>
inline Vector3D<T> operator*(float val, const Vector3D<T> &other) {
  return {other.x * val, other.y * val, other.z * val};
}
