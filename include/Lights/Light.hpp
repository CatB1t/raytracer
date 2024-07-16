#pragma once

#include "Vector/Vector3D.hpp"

class Light {
public:
  virtual Vector3D<float> calculate_lighting() = 0;
};
