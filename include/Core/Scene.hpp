#pragma once

#include <vector>
#include "Lights/Light.hpp"
#include "Shapes/Sphere.hpp"

struct Scene {
  std::vector<Light> lights;
  std::vector<Sphere> spheres;
};
