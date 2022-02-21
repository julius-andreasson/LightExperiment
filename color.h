#pragma once

#include "vec3.h"
#include "rtweekend.h"

#include <iostream>

// §inline
inline void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    // Write the translated component
    auto r = pixel_color.r;
    auto g = pixel_color.g;
    auto b = pixel_color.b;

    // Average
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(r*scale);
    g = sqrt(g*scale);
    b = sqrt(b*scale);



    out << static_cast<int>(256 * clamp(r, 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0, 0.999)) << '\n';
}