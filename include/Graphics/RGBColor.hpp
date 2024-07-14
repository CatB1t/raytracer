#pragma once

#include <algorithm>
#include <cstdint>

using PixelColorT = uint8_t;

struct RGBColor {
  PixelColorT r = 0;
  PixelColorT g = 0;
  PixelColorT b = 0;

  RGBColor operator*(float val) const {
    return {
        static_cast<PixelColorT>(std::clamp(static_cast<int>(r * val), 0, 255)),
        static_cast<PixelColorT>(std::clamp(static_cast<int>(g * val), 0, 255)),
        static_cast<PixelColorT>(std::clamp(static_cast<int>(b * val), 0, 255)),
    };
  };

  RGBColor operator+(const RGBColor& other) const {
    return {
        static_cast<PixelColorT>(std::clamp(r + other.r, 0, 255)),
        static_cast<PixelColorT>(std::clamp(g + other.g, 0, 255)),
        static_cast<PixelColorT>(std::clamp(b + other.b, 0, 255)),
    };
  };
};

inline RGBColor operator*(float val, const RGBColor &color) {
  return color * val;
};
