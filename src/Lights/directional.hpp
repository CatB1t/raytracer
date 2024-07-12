#pragma once

#include "../Point.hpp"

struct DirectionalLight {
  float intensity = 1;
  Point3D<float> direction;
};
