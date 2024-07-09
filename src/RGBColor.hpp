#pragma once

#include <cstdint>
#include <algorithm>

using PixelColorT = uint8_t;

struct RGBColor
{
    PixelColorT r = 0;
    PixelColorT g = 0;
    PixelColorT b = 0;

    RGBColor operator*(float val) const
    {
        return {
            static_cast<PixelColorT>(std::clamp(static_cast<int>(r * val), 0, 255)),
            static_cast<PixelColorT>(std::clamp(static_cast<int>(g * val), 0, 255)),
            static_cast<PixelColorT>(std::clamp(static_cast<int>(b * val), 0, 255)),
            };
    };
};

inline RGBColor operator*(float val, const RGBColor& color)
{
    return color * val;
};