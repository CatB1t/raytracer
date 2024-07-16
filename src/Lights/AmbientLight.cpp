#include "Lights/AmbientLight.hpp"
#include "Vector/Vector3D.hpp"

AmbientLight::AmbientLight(float intensity) : intensity(intensity) {}

Vector3D<float> calculcate_lighting() { return {-1, -1, -1}; }
