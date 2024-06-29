#include <iostream>
#include <algorithm>
#include <string>
#include <cstdint>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main ()
{
    const std::string output_file = "raytrace.bmp";
    constexpr int canvas_size[2] = {100, 100};
    constexpr int channels = 3;
    constexpr int data_size = canvas_size[0] * canvas_size[1] * channels;

    uint8_t data[data_size] = {};
    uint8_t background_color = 255;
    std::fill_n(data, data_size, background_color);

    int result = stbi_write_bmp(output_file.c_str(), canvas_size[0], canvas_size[1], channels, data);

    if (!result)
    {
        std::cout << "Failed writing image" << output_file << "\n";
        return -1;
    }

    std::cout << "Done Writing to " << output_file << "\n";
    return 1;
}
