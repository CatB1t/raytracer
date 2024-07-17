#include "Core/Raytracer.hpp"

namespace Raytracer {

HitInfo *intersect_objects(Vector3D origin, Vector3D dir, float t_min,
                           float t_max, const Scene &scene) {
  double closeset_sphere_distance = FLT_MAX;
  const Sphere *closeset_sphere = nullptr;

  for (auto &sphere : scene.spheres) {
    Vector2D<float> *intersect_points = sphere.intersect_ray(origin, dir);

    if (!intersect_points)
      continue;

    if (in_range(intersect_points->x, t_min, t_max) &&
        intersect_points->x < closeset_sphere_distance) {
      closeset_sphere_distance = intersect_points->x;
      closeset_sphere = &sphere;
    }

    if (in_range(intersect_points->y, t_min, t_max) &&
        intersect_points->y < closeset_sphere_distance) {
      closeset_sphere_distance = intersect_points->y;
      closeset_sphere = &sphere;
    }

    delete intersect_points;
  }

  if (!closeset_sphere)
    return nullptr;

  return new HitInfo{closeset_sphere_distance, closeset_sphere};
}

RGBColor traceRay(Vector3D origin, Vector3D dir, float t_min, float t_max,
                  Scene &scene, int depth) {
  HitInfo *hit = intersect_objects(origin, dir, t_min, t_max, scene);

  if (!hit)
    return scene.background_color;

  float total_intensity = scene.ambient_light.intensity;

  Vector3D intersection_point = origin + dir * hit->distance;
  Vector3D normal_dir = (intersection_point - hit->sphere->center).normalize();
  const double normal_length = normal_dir.length();

  Vector3D view_dir = -1 * dir;
  constexpr double eps = 0.001;

  for (auto light : scene.directional_lights) {
    HitInfo *shadow_hit = intersect_objects(intersection_point, light.direction,
                                            eps, FLT_MAX, scene);
    if (shadow_hit) {
      delete shadow_hit;
      continue;
    }
    total_intensity +=
        calculateLighting(light.direction, light.intensity, normal_dir,
                          view_dir, hit->sphere->specular);
  }

  for (auto light : scene.point_lights) {
    Vector3D light_direction = light.position - intersection_point;
    HitInfo *shadow_hit =
        intersect_objects(intersection_point, light_direction, eps, 1, scene);
    if (shadow_hit) {
      delete shadow_hit;
      continue;
    }
    total_intensity +=
        calculateLighting(light_direction, light.intensity, normal_dir,
                          view_dir, hit->sphere->specular);
  }

  RGBColor sphereColor =
      std::clamp(total_intensity, 0.0f, 1.0f) * hit->sphere->color;

  float reflective = hit->sphere->reflective;
  delete hit;

  if (depth <= 0 | reflective <= 0)
    return sphereColor;

  Vector3D reflectedDir = view_dir.reflect(normal_dir);
  RGBColor reflectColor = traceRay(intersection_point, reflectedDir, 0.05f,
                                   FLT_MAX, scene, depth - 1);
  return (reflectColor * reflective) + (sphereColor * (1.0f - reflective));
}

double calculateLighting(Vector3D light_dir, float light_intensity,
                         Vector3D normal_dir, Vector3D view_dir,
                         float specular_component) {
  double ret = 0.0f;
  double n_dot_l = normal_dir.dot(light_dir);
  if (n_dot_l > 0)
    ret += (n_dot_l / (light_dir.length()));

  Vector3D reflected_light = light_dir.reflect(normal_dir);
  double v_dot_r = view_dir.dot(reflected_light);

  if (v_dot_r > 0)
    ret += std::pow((v_dot_r / (view_dir.length() * reflected_light.length())),
                    specular_component);

  return ret * light_intensity;
}
}; // namespace Raytracer
