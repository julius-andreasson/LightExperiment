#include "vec3.h"
#include "ray.h"
#include "color.h"

#include <iostream>

// Background color
color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1);
    // Return a linear blend between two colors
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    // std::cerr << "Started" << std::endl;

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * aspect_ratio;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    std::cerr << "31";
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
    std::cerr << "33";
    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int j = 0; j < image_height; ++j) {
        if (j % 100 == 0) {
            int percent = int(100 * float(j) / (image_height - 1));
            std::cerr << percent << "\% done.\n" << std::flush;
        }
        for (int i = 0; i < image_width; ++i) {
            auto u = float(i) / (image_width - 1);
            auto v = float(j) / (image_height - 1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone!\n";
    return 0;
}