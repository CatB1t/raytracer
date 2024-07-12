#pragma once

#include "Point.hpp"
#include "RGBColor.hpp"
#include <cmath>

struct Sphere {

  float radius = 1;
  Point3D<float> center;
  RGBColor color;

  Point2D<float> *intersect_ray(Point3D<float> origin,
                                Point3D<float> direction) {
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
