#include <iostream>
#include <string>
#include <vector>
#include <cfloat>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Canvas.hpp"
#include "Point.hpp"
#include "Sphere.hpp"

struct Scene {
    std::vector<Sphere> spheres;
};

Point3D<float> canvasToViewport(Point2D<int> pixel, Canvas& canvas)
{
    float viewportWidth = 1;
    float viewportHeight = 1;
    float viewportZ = 1;

    return {
        pixel.x * (viewportWidth/canvas.width),
        pixel.y * (viewportHeight/canvas.height),
        viewportZ
    };
}

template<typename T>
bool in_range(T val, T min, T max) { return val <= max && val >= min;}

RGBColor traceRay(Point3D<float> origin, Point3D<float> dir, float t_min, float t_max, Scene& scene)
{
    float closeset_sphere_distance = FLT_MAX;
    const Sphere* closeset_sphere = nullptr;

    for(auto& sphere : scene.spheres)
    {
        Point2D<float>* intersect_points = sphere.intersect_ray(origin, dir);

        if (!intersect_points)
            continue;
        
        if(in_range(intersect_points->x, t_min, t_max) && intersect_points->x < closeset_sphere_distance)
        {
            closeset_sphere_distance = intersect_points->x;
            closeset_sphere = &sphere;
        }

        if(in_range(intersect_points->y, t_min, t_max) && intersect_points->y < closeset_sphere_distance)
        {
            closeset_sphere_distance = intersect_points->y;
            closeset_sphere = &sphere;
        }

        delete intersect_points;
    }

    if(!closeset_sphere)
        return {255, 255, 255}; // no-hit, background color
    
    return closeset_sphere->color;
}

int main ()
{
    const std::string output_file = "raytrace.bmp";
    Canvas canvas = {800, 800, 3};

    Scene scene;
    scene.spheres.push_back(Sphere{1, {0, -1, 3}, {255, 0, 0}});
    scene.spheres.push_back(Sphere{1, {-2, 0, 4}, {0, 255, 0}});
    scene.spheres.push_back(Sphere{1, {2, 0, 4}, {0, 0, 255}});
    Point3D<float> camera_origin = {0, 0, 0};

    int i_range = canvas.width / 2;
    int y_range = canvas.height / 2;

    for(int i = -i_range; i <= i_range; ++i)
    {
        for(int j = -y_range; j <= y_range; ++j)
        {
            Point3D<float> viewport_point = canvasToViewport({i, j}, canvas);
            RGBColor color = traceRay(camera_origin, viewport_point, 1, 100, scene);
            canvas.put_pixel(i, j, color);
        }
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
