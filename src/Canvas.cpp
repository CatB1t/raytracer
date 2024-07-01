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
    x = m_rangeX + x;
    y = m_rangeY - y;

    if(std::abs(x) >= this->width || std::abs(y) >= this->height)
        return;

    int p = (this->width * y + x) * this->channels;
    m_data[p] = color.r;
    m_data[p + 1] = color.g;
    m_data[p + 2] = color.b;
}