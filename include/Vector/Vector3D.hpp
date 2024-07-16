#pragma once

#include <cassert>
#include <cmath>

struct Vector3D {
  double x = 0;
  double y = 0;
  double z = 0;

  double dot(const Vector3D &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  Vector3D normalize() const {
    double sqr_sum = this->length();
    assert(std::abs(sqr_sum) > 0);
    return {x / sqr_sum, y / sqr_sum, z / sqr_sum};
  }

  Vector3D reflect(const Vector3D &normal) {
    return 2.0 * normal * this->dot(normal) - *this;
  }

  double length() const { return sqrt(x * x + y * y + z * z); }

  Vector3D operator*(double val) const { return {x * val, y * val, z * val}; }

  Vector3D operator+(const Vector3D &other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  Vector3D operator-(const Vector3D &other) const {
    return {x - other.x, y - other.y, z - other.z};
  }

  friend Vector3D operator*(double val, const Vector3D &other) {
    return {other.x * val, other.y * val, other.z * val};
  }
};
