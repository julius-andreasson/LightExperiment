#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "vec3.h"

#include <bits/types/time_t.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <ostream>
#include <time.h>
#include <execution>

hittable_list scene() {
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.4, 0.4, 0.4));
    auto material_left = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto glass   = make_shared<dielectric>(1.5);
    auto water   = make_shared<dielectric>(1.33);
    auto steel  = make_shared<metal>(color(0.4, 0.4, 0.5), 0.3);
    auto gold  = make_shared<metal>(color(212.0 / 255, 175.0 / 255, 55.0 / 255), 0.0);
    auto fuzzy_gold  = make_shared<metal>(color(205.0 / 255, 127.0 / 255, 50.0 / 255), 0.5);
    auto lamb_red  = make_shared<lambertian>(color(1.0, 0.0, 0.0));
    auto lamb_green  = make_shared<lambertian>(color(0.0, 1.0, 0.0));

    // 1st big ball
    float r = 2;
    float ty = 1.25;
    float tx = sqrt(r*r + ty*ty);
    point3 pos(tx - 0.3, -0.7, 0);
    shared_ptr<material> currMat = gold;
    world.add(make_shared<sphere>(pos, r, currMat));
    // 2nd big ball
    r = 1.25;
    pos = point3(-0.3, 1.5, 0);
    currMat = steel;
    world.add(make_shared<sphere>(pos, r, currMat));
    // 3rd big ball
    r = 2;
    pos = point3(-1.6, -2, 1.0);
    currMat = glass;
    world.add(make_shared<sphere>(pos, r, currMat));
    // making it hollow
    r = -1.9;
    world.add(make_shared<sphere>(pos, r, currMat));
    // cross-section ball
    r = 0.5;
    pos = point3(0.9, 1.2, 2.0);
    currMat = glass;
    world.add(make_shared<sphere>(pos, r, currMat));
    // 'top-right of 2nd big ball'-ball
    r = 0.4;
    pos = point3(-0.6, 2.0, 2.0);
    currMat = gold;
    world.add(make_shared<sphere>(pos, r, currMat));
    
    for (int x = -1; x < 7; x++) {
        for (int y = -6; y < 7; y++) {
            r = 0.1 + 0.7 * random_float();
            pos = point3(1.6*x, 1.6*y, -4 + random_float());
            currMat = make_shared<metal>(color(random_float(), random_float(), random_float()), random_float());
            world.add(make_shared<sphere>(pos, r, currMat));
        }
    }
    return world;
}

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
    }
    // Background color
    vec3 unit_direction = unit_vector(r.direction());
    auto y = 0.5 * (sin(unit_direction.y*3) + 1.0); 
    return (1 - y) * color(.1, .1, 1) + y * color(1, 1, 1);
}

void colorPixel(float u, float v, ray r, hittable_list world, int max_bounces, int samples_per_pixel) {
  color pixel_color = color(0, 0, 0);
  // run multiple samples
  for (int s = 0; s < samples_per_pixel; ++s) {
    pixel_color += ray_color(r, world, max_bounces);
  }
  write_color(std::cout, pixel_color, samples_per_pixel);
}

int main() {
    std::cerr << "Rendering job started.\n";
    time_t t0 = time(&t0);

    // Image
    const auto aspect_ratio = 1080.0 / 2340.0;
    const int image_width = 300; // 1080 x 2340 p
    const int image_height = static_cast<int>(image_width / aspect_ratio); 
    const int samples_per_pixel = 20;
    const int max_bounces = 16;

    // Camera
    point3 lookfrom = point3(-15, 0, 25);
    point3 lookat = point3(0, 0, 0);
    vec3 vup(0, 1, 0); // "view up" vector
    float vfov = 20; // Vertical field of view, degrees
    // 150 mm, 500 mm away => 16.7 degrees vfov
    float aperture = 0.05;
    float focus_dist = (lookfrom - lookat).length();
    std::cerr << "Focus distance: " << focus_dist << "\n";
    // Create a camera with all of these settings. 
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist);
    
    // World
    hittable_list world = scene();
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j) {
        // std::cerr << (j + 1) << " out of " << image_height << " rows done.\n";
        if (j % 10 == 0) {
            float percent = (1 - (float(j) / (image_height - 1)));
            time_t t1 = time(&t1);
            float t = t1 - t0;
            std::cerr << int(percent*100) << "\% done. " 
                << t << " s passed, est. " << int(t / percent - t) << " s remaining.\n" << std::flush;
        }
        for (int i = 0; i < image_width; ++i) {
          auto u = float(i + random_float()) / (image_width - 1);
          auto v = float(j + random_float()) / (image_height - 1);
          ray r = cam.get_ray(u, v);
          // std::for_each(std::execution::par);
          colorPixel(u, v, r, world, max_bounces, samples_per_pixel);
        }
    }

    // Finish
    time_t t1 = time(&t1);
    std::cerr << "\nRendering job complete!\nTotal time taken: " << t1 - t0 << " seconds." << std::endl;
    return 0;
}