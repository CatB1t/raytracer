#pragma once

#include "Light.hpp"
#include "Vector/Vector3D.hpp"

class PointLight : public Light {
public:
  PointLight(float intensity, Vector3D<float> position);
  float intensity = 1;
  Vector3D<float> position;
  Vector3D<float> calculate_lighting() override;
};
