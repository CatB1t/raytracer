#include "Lights/PointLight.hpp"

PointLight::PointLight(float intensity, Vector3D<float> position)
    : intensity(intensity), position(position) {}

Vector3D<float> PointLight::calculate_lighting()
{
  return {-2, -2, -2};
}
