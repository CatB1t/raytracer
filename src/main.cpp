#include <cstdlib>
#include <iostream>
#include <filesystem>

#include "Configuration/LuaConfigHandler.hpp"
#include "Core/CmdParser.hpp"
#include "Core/Raytracer.hpp"
#include "Core/Scene.hpp"
#include "Graphics/BmpImage.hpp"
#include "Graphics/Canvas.hpp"
#include "Vector/Vector3D.hpp"
#include "Util/Timer.hpp"

int main(int argc, char *argv[]) {
  CmdParser parser = {argc, argv};

  std::filesystem::path parent_dir = std::filesystem::path(parser.getOptStr("_exec_path_")).parent_path();

  unsigned int width = parser.getOptUint("-w");
  unsigned int height = parser.getOptUint("-h");
  std::filesystem::path image_path = std::filesystem::path(parent_dir).append(parser.getOptStr("-o"));
  BmpImage image = {image_path, width, height, 3};

  std::filesystem::path read_config_path = std::filesystem::path(parent_dir);
  LuaConfigHandler cfg_handler {};
  if(parser.isValidOption("-g")) {
    read_config_path /= parser.getOptStr("-g");
    if(!cfg_handler.generate_example_config(read_config_path))
      return EXIT_FAILURE;
  }
  else {
    read_config_path /= parser.getOptStr("-c");
  }

  Scene scene;
  if(!cfg_handler.read_config(read_config_path.c_str(), &scene)) {
    printf("Failed to load scene configuration.\n");
    return EXIT_FAILURE;
  }

  Canvas &canvas = image.getCanvas();
  int x_range = canvas.get_rangeX();
  int y_range = canvas.get_rangeY();

  Timer timer;
  timer.start();

  for (int i = -x_range; i <= x_range; ++i) {
    for (int j = -y_range; j <= y_range; ++j) {
      Vector3D viewport_point = canvas.canvasToViewport({i, j});
      RGBColor color = Raytracer::traceRay(scene.camera_position, viewport_point, 1, 100, scene, scene.tracing_depth);
      canvas.put_pixel(i, j, color);
    }
  }

  timer.end();
  std::cout << "Runtime: " << timer.getTime().count() << "ms. ";

  image.write();
  return EXIT_SUCCESS;
}
