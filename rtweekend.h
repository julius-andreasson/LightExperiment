#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
// Common Headers
#include "ray.h"
#include "vec3.h"

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.14159265;

// Utilty functions

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0;
}

inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float(float min, float max) {
    return min + (max-min)*random_float();
}

// § supposed to be in vec3
inline vec3 random_vec3() {
    return vec3(random_float(), random_float(), random_float());
}
// § supposed to be in vec3
inline vec3 random_vec3(float min, float max) {
    return vec3(random_float(min, max), random_float(min, max), random_float(min,max));
}
// § supposed to be in vec3
inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = random_vec3();
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_float(-1, 1), random_float(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}