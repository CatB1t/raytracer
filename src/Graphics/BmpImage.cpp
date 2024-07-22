#include "Graphics/BmpImage.hpp"
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

BmpImage::BmpImage(const std::string &name, unsigned int width,
                   unsigned int height, unsigned int channels)
    : m_name(name), m_canvas({width, height, channels}) {}

bool BmpImage::write() {
  std::string name_on_disk = m_name + ".bmp";

  int result =
      stbi_write_bmp(name_on_disk.c_str(), m_canvas.width, m_canvas.height,
                     m_canvas.channels, m_canvas.get_data());
  if (!result) {
    std::cout << "Failed writing image" << name_on_disk << "\n";
    return false;
  }

  std::cout << "Done Writing to " << name_on_disk << "\n";
  return true;
}
