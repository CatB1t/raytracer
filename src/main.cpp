#include "Core/Raytracer.hpp"
#include "Core/Scene.hpp"
#include "Graphics/BmpImage.hpp"
#include "Graphics/Canvas.hpp"
#include "Lights/DirectionalLight.hpp"
#include "Lights/PointLight.hpp"
#include "Shapes/Sphere.hpp"
#include "Vector/Vector3D.hpp"

int main() {
  BmpImage image = {"image", 800, 800, 3};

  Scene scene{{0, 0, 0}, 0.2f};

  scene.spheres.push_back(Sphere{1, {0, -1, 3}, {255, 0, 0}, 500.0f, 0.2f});
  scene.spheres.push_back(Sphere{1, {-2, 0, 4}, {0, 255, 0}, 10.0f, 0.4f});
  scene.spheres.push_back(Sphere{1, {2, 0, 4}, {0, 0, 255}, 500.0f, 0.3f});
  scene.spheres.push_back(Sphere{5000, {0, -5001, 0}, {255, 255, 0}, 1000.0f, 0.5f});

  scene.point_lights.push_back(PointLight{0.6f, {2, 1, 0}});
  scene.directional_lights.push_back(DirectionalLight{0.2f, {1, 4, 4}});

  Vector3D camera_origin = {0, 0, 0};

  Canvas &canvas = image.getCanvas();
  int i_range = canvas.width / 2;
  int y_range = canvas.height / 2;

  for (int i = -i_range; i <= i_range; ++i) {
    for (int j = -y_range; j <= y_range; ++j) {
      Vector3D viewport_point = canvas.canvasToViewport({i, j});
      RGBColor color = Raytracer::traceRay(camera_origin, viewport_point, 1, 100, scene, 10);
      canvas.put_pixel(i, j, color);
    }
  }

  image.write();
  return 1;
}
