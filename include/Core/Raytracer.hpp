#pragma once

#include "Core/Scene.hpp"
#include "Shapes/Sphere.hpp"
#include <cfloat>

namespace Raytracer {

struct HitInfo {
  double distance = -1.0;
  const Sphere *sphere = nullptr;
};

template <typename T> bool in_range(T val, T min, T max) {
  return val <= max && val >= min;
}

double calculateLighting(Vec3 light_dir, float light_intensity, Vec3 normal_dir,
                         Vec3 view_dir, float specular_component);

RGBColor traceRay(Vec3 origin, Vec3 dir, float t_min, float t_max, Scene &scene,
                  int depth);

HitInfo *intersect_objects(Vec3 origin, Vec3 dir, float t_min, float t_max,
                           const Scene &scene);

}; // namespace Raytracer
