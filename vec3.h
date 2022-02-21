#pragma once

#include <cmath>
#include <iostream>
#include <math.h>
#include <ostream>

class vec3 {
    public:
        vec3() : e{0,0,0} {}
        vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

        vec3 operator-() const { return vec3(-x, -y, -z); }
        float operator[](int i) const { return e[i]; }
        float& operator[](int i) { return e[i]; }

        // Returns element-wise sum of this and vec3 v
        vec3& operator+=(const vec3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        // Returns itself multiplied by the scalar s
        vec3& operator*=(const float s) {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }
        
        // Returns itself divided by the scalar s
        vec3& operator/=(const float s) {
            return *this *= 1/s;
        }

        float length() const {
            return std::sqrt(length_squared());
        }

        float length_squared() const {
            return x*x + y*y + z*z;
        }

        bool near_zero() const {
            const auto lim = 1e-8;
            return (fabsf(x) < lim && fabsf(y) < lim && fabsf(z) < lim);
        }

        // random_sphere functions supposed to be here put in rtweekend.h
        // to avoid redefinition
    public:
        union {
            float e[3];
            struct{
                float x, y, z;
            };
            struct{
                float r, g, b;
            };
        };
};

// Type aliases to avoid mix-up, since this class is used for both colors and positions.
using point3 = vec3; // 3D-point
using color = vec3; // RGB color

// vec3 Utility functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(float s, const vec3 &u) {
    return vec3(u.x * s, u.y * s, u.z * s);
}

inline vec3 operator*(const vec3 &u, float s) {
    return s * u;
}

inline vec3 operator/(vec3 u, float s) {
    return (1/s) * u;
}

inline vec3 operator/(float s, vec3 u) {
    return u / s;
}

inline float dot(const vec3 &u, const vec3 &v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(n, v)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);//(uv * cos_theta*n);
    vec3 r_out_parallell = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallell;
}