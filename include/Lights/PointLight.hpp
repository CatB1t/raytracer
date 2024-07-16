#pragma once

#include "Vector/Vector3D.hpp"

struct PointLight {
public:
  float intensity = 1;
  Vector3D<float> position;
};
