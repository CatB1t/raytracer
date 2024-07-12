#pragma once

#include "RGBColor.hpp"
#include <vector>

class Canvas {
public:
  const unsigned int width;
  const unsigned int height;
  const unsigned int channels;

  Canvas(unsigned int width, unsigned int height, unsigned int channels);
  ~Canvas() = default;

  void put_pixel(int x, int y, RGBColor color);
  const PixelColorT *get_data() { return m_data.data(); };

private:
  std::vector<PixelColorT> m_data;
  int m_rangeX = 0;
  int m_rangeY = 0;
};
