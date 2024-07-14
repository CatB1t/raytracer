#pragma once

#include "../Vector/Vector3D.hpp"

struct DirectionalLight {
  float intensity = 1;
  Point3D<float> direction;
};
