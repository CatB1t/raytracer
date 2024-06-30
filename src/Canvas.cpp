#include <cmath>
#include "Canvas.hpp"

Canvas::Canvas(unsigned int width, unsigned int height, unsigned int channels) 
    : width{width}, height{height}, channels(channels) 
{
    m_data.resize(this->width * this->height * this->channels);
    m_rangeX = this->width / 2;
    m_rangeY = this->height / 2;
};

void Canvas::put_pixel(int x, int y, RGBColor color)
{
    if(std::abs(x) > m_rangeX || std::abs(y) > m_rangeY)
        return;

    x = m_rangeX + x;
    y = m_rangeY - y;

    // TODO doesn't work with even dimensions
    int p = this->width * y * this->channels + x * this->channels;
    m_data[p] = color.r;
    m_data[p + 1] = color.g;
    m_data[p + 2] = color.b;
}