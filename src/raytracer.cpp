#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Vendor/stb_image_write.h"

#include "Core/Scene.hpp"
#include "Graphics/Canvas.hpp"
#include "Shapes/Sphere.hpp"
#include "Vector/Vector2D.hpp"
#include "Vector/Vector3D.hpp"
#include "Lights/ambient.hpp"
#include "Lights/directional.hpp"
#include "Lights/point.hpp"

template <typename T> bool in_range(T val, T min, T max) {
  return val <= max && val >= min;
}

float calculateLighting(Point3D<float> light_dir, Point3D<float> normal_dir,
                        Point3D<float> view_dir, float light_intensity,
                        float specular_component) {
  float ret = 0.0f;
  float n_dot_l = normal_dir.dot(light_dir);
  if (n_dot_l > 0)
    ret += (n_dot_l / (light_dir.length()));

  Point3D<float> reflected_light = light_dir.reflect(normal_dir);
  float v_dot_r = view_dir.dot(reflected_light);

  if (v_dot_r > 0)
    ret += std::pow((v_dot_r / (view_dir.length() * reflected_light.length())), specular_component);

  return ret * light_intensity;
}

auto find_nearest_intersection(Point3D<float> origin, Point3D<float> dir,
                               float t_min, float t_max, const Scene &scene) {
  float closeset_sphere_distance = FLT_MAX;
  const Sphere *closeset_sphere = nullptr;

  for (auto &sphere : scene.spheres) {
    Point2D<float> *intersect_points = sphere.intersect_ray(origin, dir);

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

  return std::make_pair(closeset_sphere_distance, closeset_sphere);
}

RGBColor traceRay(Point3D<float> origin, Point3D<float> dir, float t_min,
                  float t_max, Scene &scene, int depth) {
  auto hit_point = find_nearest_intersection(origin, dir, t_min, t_max, scene);

  if (!hit_point.second)
    return {0, 0, 0}; // no-hit, background color

  float closeset_sphere_distance = hit_point.first;
  const Sphere *closeset_sphere = hit_point.second;
  float total_intensity = scene.ambient_light.intensity;

  Point3D<float> intersection_point = origin + dir * closeset_sphere_distance;
  Point3D<float> normal_dir = (intersection_point - closeset_sphere->center).normalize();
  const float normal_length = normal_dir.length();

  Point3D<float> view_dir = -1 * dir;
  constexpr float eps = 0.01f;

  for (auto light : scene.directional_lights) {
    auto hit_point = find_nearest_intersection(intersection_point, light.direction, eps, FLT_MAX, scene);
    if (!hit_point.second) // There's no object between light and point
      total_intensity += calculateLighting(light.direction, normal_dir, view_dir, light.intensity, closeset_sphere->specular);
  }

  for (auto light : scene.point_lights) {
    Point3D<float> light_dir = light.position - intersection_point;

    auto hit_point = find_nearest_intersection(intersection_point, light_dir, eps, 1, scene);
    if (!hit_point.second) // There's no object between light and point
      total_intensity += calculateLighting(light_dir, normal_dir, view_dir, light.intensity, closeset_sphere->specular);
  }

  RGBColor sphereColor = std::clamp(total_intensity, 0.0f, 1.0f) * closeset_sphere->color;

  if (depth <= 0 | closeset_sphere->reflective <= 0)
    return sphereColor;

  Point3D<float> reflectedDir = view_dir.reflect(normal_dir);
  RGBColor reflectColor = traceRay(intersection_point, reflectedDir, 0.05f, FLT_MAX, scene, depth - 1);
  return (reflectColor * closeset_sphere->reflective) + (sphereColor * (1.0f - closeset_sphere->reflective));
}

int main() {
  const std::string output_file = "raytrace.bmp";
  Canvas canvas = {800, 800, 3};

  Scene scene;
  scene.spheres.push_back(Sphere{1, {0, -1, 3}, {255, 0, 0}, 500.0f, 0.2f});
  scene.spheres.push_back(Sphere{1, {-2, 0, 4}, {0, 255, 0}, 10.0f, 0.4f});
  scene.spheres.push_back(Sphere{1, {2, 0, 4}, {0, 0, 255}, 500.0f, 0.3f});
  scene.spheres.push_back(Sphere{5000, {0, -5001, 0}, {255, 255, 0}, 1000.0f, 0.5f});

  scene.point_lights.push_back(PointLight{0.6f, {2, 1, 0}});
  scene.directional_lights.push_back(DirectionalLight{0.2f, {1, 4, 4}});

  Point3D<float> camera_origin = {0, 0, 0};

  int i_range = canvas.width / 2;
  int y_range = canvas.height / 2;

  for (int i = -i_range; i <= i_range; ++i) {
    for (int j = -y_range; j <= y_range; ++j) {
      Point3D<float> viewport_point = canvas.canvasToViewport({i, j});
      RGBColor color = traceRay(camera_origin, viewport_point, 1, 100, scene, 10);
      canvas.put_pixel(i, j, color);
    }
  }

  int result = stbi_write_bmp(output_file.c_str(), canvas.width, canvas.height,
                              canvas.channels, canvas.get_data());

  if (!result) {
    std::cout << "Failed writing image" << output_file << "\n";
    return -1;
  }

  std::cout << "Done Writing to " << output_file << "\n";
  return 1;
}
