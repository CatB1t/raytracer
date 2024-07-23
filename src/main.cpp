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

  unsigned int width = parser.getOptUint("-w");
  unsigned int height = parser.getOptUint("-h");
  BmpImage image = {parser.getOptStr("-o"), width, height, 3};

  std::filesystem::path parent_dir = std::filesystem::path(parser.getOptStr("_exec_path_")).parent_path();
  std::filesystem::path read_config_path = std::filesystem::path(parent_dir);

  LuaConfigHandler cfg_handler {};
  if(parser.isValidOption("-g")) {
    read_config_path /= parser.getOptStr("-g");
    cfg_handler.generate_example_config(read_config_path);
  }
  else {
    read_config_path /= parser.getOptStr("-c");
  }

  if(!cfg_handler.read_config(read_config_path.c_str())) {
    printf("Failed to load scene configuration.\n");
    return -1;
  }

  Scene *scene = cfg_handler.getScene();

  Canvas &canvas = image.getCanvas();
  int x_range = canvas.get_rangeX();
  int y_range = canvas.get_rangeY();

  Timer timer;
  timer.start();

  for (int i = -x_range; i <= x_range; ++i) {
    for (int j = -y_range; j <= y_range; ++j) {
      Vector3D viewport_point = canvas.canvasToViewport({i, j});
      RGBColor color = Raytracer::traceRay(scene->camera_position, viewport_point, 1, 100, *scene, scene->tracing_depth);
      canvas.put_pixel(i, j, color);
    }
  }

  timer.end();
  std::cout << "Runtime: " << timer.getTime().count() << "ms. ";
  delete scene;

  image.write();
  return 1;
}
