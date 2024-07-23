#include <iostream>

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

  // TODO search file path properly
  LuaConfigHandler cfg_handler;
  if(!cfg_handler.read_config(std::string("cfg.lua"))) {
    printf("Failed to load scene configuration.\n");
    return -1;
  }

  Scene *scene = cfg_handler.getScene();

  Canvas &canvas = image.getCanvas();
  int i_range = canvas.width / 2;
  int y_range = canvas.height / 2;

  Timer timer;
  timer.start();

  for (int i = -i_range; i <= i_range; ++i) {
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
