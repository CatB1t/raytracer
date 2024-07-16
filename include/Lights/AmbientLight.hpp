#pragma once

#include "Light.hpp"
#include "Vector/Vector3D.hpp"

class AmbientLight : public Light {
public:
  AmbientLight(float intensity);
  float intensity = 0.1;
  Vector3D<float> calculate_lighting() override;
};
