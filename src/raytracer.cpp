#include <iostream>
#include <string>
#include <vector>
#include <cfloat>
#include <algorithm>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Canvas.hpp"
#include "Point.hpp"
#include "Sphere.hpp"
#include "Lights/ambient.hpp"
#include "Lights/point.hpp"
#include "Lights/directional.hpp"

struct Scene {
    AmbientLight ambient_light {0.2};
    std::vector<PointLight> point_lights;
    std::vector<DirectionalLight> directional_lights;
    std::vector<Sphere> spheres;
};

Point3D<float> canvasToViewport(const Point2D<int>& pixel, const Canvas& canvas)
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

float calculateLighting(Point3D<float> light_dir, Point3D<float> normal_dir, float light_intensity) {
  float n_dot_l = normal_dir.dot(light_dir);
  if(n_dot_l < 0)
      return 0.0f;
  return light_intensity * (n_dot_l / (light_dir.length()));
}

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
    
    float total_intensity = scene.ambient_light.intensity;

    Point3D<float> intersection_point = origin + dir * closeset_sphere_distance;
    Point3D<float> normal_dir = (intersection_point - closeset_sphere->center).normalize();
    const float normal_length = normal_dir.length();

    for(auto light : scene.directional_lights)
      total_intensity += calculateLighting(light.direction, normal_dir, light.intensity);

    for(auto light : scene.point_lights)
    {
        Point3D<float> light_dir = light.position - intersection_point;
        total_intensity += calculateLighting(light_dir, normal_dir, light.intensity);
    }

    return std::clamp(total_intensity, 0.0f, 1.0f) * closeset_sphere->color;
}

int main ()
{
    const std::string output_file = "raytrace.bmp";
    Canvas canvas = {800, 800, 3};

    Scene scene;
    scene.spheres.push_back(Sphere{1, {0, -1, 3}, {255, 0, 0}});
    scene.spheres.push_back(Sphere{1, {-2, 0, 4}, {0, 255, 0}});
    scene.spheres.push_back(Sphere{1, {2, 0, 4}, {0, 0, 255}});
    scene.spheres.push_back(Sphere{5000, {0, -5001, 0}, {255, 255, 0}});

    scene.point_lights.push_back(PointLight{0.6f, {2, 1, 0}});
    scene.directional_lights.push_back(DirectionalLight{0.2f, {1, 4, 4}});

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
