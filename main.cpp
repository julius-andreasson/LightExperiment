#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

// Ray color calc
color ray_color(const ray& r, const hittable& world, int max_bounces_remaining) {
    hit_record rec;

    // if max-depth exceeded, no more light should be gathered.
    if (max_bounces_remaining <= 0) {
        return color(0, 0, 0);
    }

    // ignore hits very near zero
    if (world.hit(r, 0.001, infinity, rec)) {
        // recursive call, creating a new ray going from the hit-point in a new direction
        // (towards target, which is a random-aimed bounce off of the hit object)
        point3 target = rec.p + rec.normal + random_unit_vector();//random_in_unit_sphere();
        ray scattered;
        color attentuation;
        if (rec.mat_ptr->scatter(r, rec, attentuation, scattered))
            return attentuation * ray_color(scattered, world, max_bounces_remaining - 1);
        return color(0, 0, 0);
        // return 0.5 * ray_color(ray(rec.p, target - rec.p), world, max_bounces_remaining - 1);
    }
    // Background color
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    // Return a linear blend between two colors
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    std::cerr << "Started" << std::endl;

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_bounces = 50;

    // World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_left = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto glass   = make_shared<dielectric>(1.5);
    auto steel  = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);
    auto gold  = make_shared<metal>(color(205.0 / 255, 127.0 / 255, 50.0 / 255), 0.0);
    auto fuzzy_gold  = make_shared<metal>(color(205.0 / 255, 127.0 / 255, 50.0 / 255), 0.5);
    auto material_right_red  = make_shared<lambertian>(color(1.0, 0.0, 0.0));

    world.add(make_shared<sphere>(point3(0, -1000.5, -1), 1000, material_ground)); // Ground
    // world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left)); // left sphere
    world.add(make_shared<sphere>(point3( -1.2, 0, -1), 0.5, glass)); // mid sphere
    world.add(make_shared<sphere>(point3( -1.2, 0, -1), -0.4, glass)); // mid sphere
    world.add(make_shared<sphere>(point3( 1, 1.1, -1.2), 0.5, glass)); // top-right sphere
    world.add(make_shared<sphere>(point3( 1, 1.1, -1.2), -0.4, glass)); // top-right sphere
    world.add(make_shared<sphere>(point3( -0.3, 1.1, -1.2), 0.5, steel)); // top sphere
    world.add(make_shared<sphere>(point3( 0, 0, -1), 0.5, gold)); // right sphere
    world.add(make_shared<sphere>(point3( 1.2, 0, -1), 0.5, fuzzy_gold)); // right sphere
    world.add(make_shared<sphere>(point3( 7, 2.5, -5), 2.5, material_right_red)); // right-back sphere

    // Camera
    camera cam(90, 16.0 / 9.0);
    
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j) {
        // std::cerr << (j + 1) << " out of " << image_height << " rows done.\n";
        if (j % 10 == 0) {
            int percent = int(100 * (1 - (float(j) / (image_height - 1))));
            std::cerr << percent << "\% done.\n" << std::flush;
        }
        for (int i = 0; i < image_width; ++i) {
            color pixel_color = color(0, 0, 0);
            // run multiple samples
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = float(i + random_float()) / (image_width - 1);
                auto v = float(j + random_float()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_bounces);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    // Finish
    std::cerr << "\nDone!\n";
    return 0;
}