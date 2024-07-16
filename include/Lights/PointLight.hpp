#pragma once

#include "Vector/Vector3D.hpp"

class PointLight {
public:
  PointLight(float intensity, Vector3D<float> position);
  float intensity = 1;
  Vector3D<float> position;
  Vector3D<float> calculate_lighting();
};
