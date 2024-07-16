#pragma once

#include "Vector/Vector2D.hpp"
#include "Vector/Vector3D.hpp"
#include "Graphics/RGBColor.hpp"
#include <cmath>

struct Sphere {

  float radius = 1;
  Vector3D<float> center;
  RGBColor color;
  float specular = -1.0f;
  float reflective = -1.0f;

  Vector2D<float> *intersect_ray(Vector3D<float> origin,
                                Vector3D<float> direction) const {
    Vector3D<float> co = origin - center;
    float a = direction.dot(direction);
    float b = 2 * direction.dot(co);
    float c = co.dot(co) - radius * radius;

    float disc = b * b - 4 * a * c;
    if (disc < 0)
      return nullptr;

    disc = sqrt(disc);
    Vector2D<float> *hit = new Vector2D<float>();
    hit->x = (-b + disc) / (2 * a);
    hit->y = (-b - disc) / (2 * a);
    return hit;
  };
};
