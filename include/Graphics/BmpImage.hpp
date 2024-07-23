#pragma once

#include <filesystem>

#include "Graphics/Canvas.hpp"
#include <string>

class BmpImage {
public:
  BmpImage(std::filesystem::path &image_path, unsigned int width, unsigned int height, unsigned int channels);
  bool write();
  Canvas &getCanvas() { return m_canvas; }

private:
  std::filesystem::path m_imagepath;
  Canvas m_canvas;
};
