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

double calculateLighting(Vector3D light_dir, float light_intensity,
                         Vector3D normal_dir, Vector3D view_dir,
                         float specular_component);

RGBColor traceRay(Vector3D origin, Vector3D dir, float t_min, float t_max,
                  Scene &scene, int depth);

HitInfo *intersect_objects(Vector3D origin, Vector3D dir, float t_min,
                           float t_max, const Scene &scene);

}; // namespace Raytracer
