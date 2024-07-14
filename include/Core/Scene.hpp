#pragma once

#include <vector>
#include "Shapes/Sphere.hpp"
#include "Lights/point.hpp"
#include "Lights/ambient.hpp"
#include "Lights/directional.hpp"

struct Scene {
  AmbientLight ambient_light{0.2};
  std::vector<PointLight> point_lights;
  std::vector<DirectionalLight> directional_lights;
  std::vector<Sphere> spheres;
};
