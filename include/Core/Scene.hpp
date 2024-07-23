#pragma once

#include <vector>
#include "Graphics/RGBColor.hpp"
#include "Lights/AmbientLight.hpp"
#include "Lights/PointLight.hpp"
#include "Lights/DirectionalLight.hpp"
#include "Shapes/Sphere.hpp"
#include "Vector/Vector3D.hpp"

struct Scene {
  int tracing_depth = 3; // TODO should not be here
  Vector3D camera_position {0, 0, 0};
  RGBColor background_color;
  AmbientLight ambient_light;
  std::vector<PointLight> point_lights;
  std::vector<DirectionalLight> directional_lights;
  std::vector<Sphere> spheres;
};
