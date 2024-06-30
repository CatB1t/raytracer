#include <iostream>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Canvas.hpp"

int main ()
{
    const std::string output_file = "raytrace.bmp";
    Canvas canvas = {25, 25, 3};

    // Draw a diagonal line
    for(int j = -12; j < 13; ++j)
    {
        canvas.put_pixel(j, j, {255, 255, 255});
    }

    int result = stbi_write_bmp(output_file.c_str(), canvas.width, canvas.height, canvas.channels, canvas.get_data());

    if (!result)
    {
        std::cout << "Failed writing image" << output_file << "\n";
        return -1;
    }

    std::cout << "Done Writing to " << output_file << "\n";
    return 1;
}
