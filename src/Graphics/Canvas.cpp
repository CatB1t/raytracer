#include "Graphics/Canvas.hpp"
#include <cmath>

Canvas::Canvas(unsigned int width, unsigned int height, unsigned int channels)
    : width{width}, height{height}, channels(channels), m_rangeX (width/2), m_rangeY(height/2) {
  m_data.resize(this->width * this->height * this->channels);
};

void Canvas::put_pixel(int x, int y, RGBColor color) {
  x = m_rangeX + x;
  y = m_rangeY - y;

  if (std::abs(x) >= this->width || std::abs(y) >= this->height)
    return;

  int p = (this->width * y + x) * this->channels;
  m_data[p] = color.r;
  m_data[p + 1] = color.g;
  m_data[p + 2] = color.b;
}

Vec3 Canvas::canvasToViewport(const Vector2D<int> &pixel) {
  float viewportWidth = 1;
  float viewportHeight = 1;
  float viewportZ = 1;

  return {pixel.x * (viewportWidth / this->width),
          pixel.y * (viewportHeight / this->height), viewportZ};
}
