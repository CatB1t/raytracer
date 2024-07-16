#pragma once

#include <vector>
#include "Graphics/RGBColor.hpp"
#include "Lights/AmbientLight.hpp"
#include "Lights/PointLight.hpp"
#include "Lights/DirectionalLight.hpp"
#include "Shapes/Sphere.hpp"

struct Scene {
  RGBColor background_color;
  AmbientLight ambient_light;
  std::vector<PointLight> point_lights;
  std::vector<DirectionalLight> directional_lights;
  std::vector<Sphere> spheres;
};
