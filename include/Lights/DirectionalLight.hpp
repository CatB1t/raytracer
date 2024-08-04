#pragma once

#include "Vector/Vector3D.hpp"

struct DirectionalLight {
public:
  float intensity = 1;
  Vec3 direction;
};
