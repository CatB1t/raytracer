#pragma once

#include "Vector/Vector2D.hpp"
#include "Vector/Vector3D.hpp"
#include "Graphics/RGBColor.hpp"
#include <cmath>

struct Sphere {

  float radius = 1;
  Point3D<float> center;
  RGBColor color;
  float specular = -1.0f;
  float reflective = -1.0f;

  Point2D<float> *intersect_ray(Point3D<float> origin,
                                Point3D<float> direction) const {
    Point3D<float> co = origin - center;
    float a = direction.dot(direction);
    float b = 2 * direction.dot(co);
    float c = co.dot(co) - radius * radius;

    float disc = b * b - 4 * a * c;
    if (disc < 0)
      return nullptr;

    disc = sqrt(disc);
    Point2D<float> *hit = new Point2D<float>();
    hit->x = (-b + disc) / (2 * a);
    hit->y = (-b - disc) / (2 * a);
    return hit;
  };
};
