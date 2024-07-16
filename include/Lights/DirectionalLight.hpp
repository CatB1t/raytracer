#pragma once

#include "Light.hpp"
#include "Vector/Vector3D.hpp"

class DirectionalLight : public Light {
public:
  DirectionalLight(float intensity, Vector3D<float> direction);
  float intensity = 1;
  Vector3D<float> direction;
  Vector3D<float> calculate_lighting() override;
};
