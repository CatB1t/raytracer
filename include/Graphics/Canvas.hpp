#pragma once

#include <vector>
#include "Graphics/RGBColor.hpp"
#include "Vector/Vector2D.hpp"
#include "Vector/Vector3D.hpp"

class Canvas {
public:
  const unsigned int width;
  const unsigned int height;
  const unsigned int channels;

  Canvas(unsigned int width, unsigned int height, unsigned int channels);
  ~Canvas() = default;

  void put_pixel(int x, int y, RGBColor color);
  const PixelColorT *get_data() { return m_data.data(); };
  Vector3D canvasToViewport(const Vector2D<int> &pixel);
  unsigned int get_rangeX() const { return m_rangeX; };
  unsigned int get_rangeY() const { return m_rangeY; };

private:
  std::vector<PixelColorT> m_data;
  const unsigned int m_rangeX;
  const unsigned int m_rangeY;
};
