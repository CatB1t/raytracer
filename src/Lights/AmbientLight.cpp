#include "Lights/AmbientLight.hpp"

AmbientLight::AmbientLight(float intensity) : intensity(intensity) {}

Vector3D<float> AmbientLight::calculate_lighting() { return {-1, -1, -1}; }
