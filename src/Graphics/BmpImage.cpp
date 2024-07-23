#include "Graphics/BmpImage.hpp"
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

BmpImage::BmpImage(std::filesystem::path &image_path, unsigned int width,
                   unsigned int height, unsigned int channels)
    : m_imagepath(image_path), m_canvas({width, height, channels}) {}

bool BmpImage::write() {
  m_imagepath.replace_extension(".bmp");

  int result =
      stbi_write_bmp(m_imagepath.c_str(), m_canvas.width, m_canvas.height,
                     m_canvas.channels, m_canvas.get_data());
  if (!result) {
    std::cout << "Failed writing image" << m_imagepath << "\n";
    return false;
  }

  std::cout << "Done Writing to " << m_imagepath << "\n";
  return true;
}
