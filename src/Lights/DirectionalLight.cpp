#include "Lights/DirectionalLight.hpp"

DirectionalLight::DirectionalLight(float intensity, Vector3D<float> direction)
    : intensity(intensity), direction(direction){};

Vector3D<float> DirectionalLight::calculate_lighting() { return {-1, -1, -1}; }
