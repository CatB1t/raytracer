#pragma once

#include "../Vector/Vector3D.hpp"

struct PointLight {
  float intensity = 1;
  Point3D<float> position;
};
