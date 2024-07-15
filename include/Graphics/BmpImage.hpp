#pragma once

#include "Graphics/Canvas.hpp"
#include <string>

class BmpImage {
public:
  BmpImage(const std::string &name, unsigned int width, unsigned int height, unsigned int channels);
  bool write();
  Canvas &getCanvas() { return m_canvas; }

private:
  std::string m_name;
  Canvas m_canvas;
};
