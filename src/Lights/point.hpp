#pragma once

#include "../Point.hpp"

struct PointLight {
  float intensity = 1;
  Point3D<float> position;
};
