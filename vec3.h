#pragma once

#include <cmath>
#include <iostream>
#include <math.h>
#include <ostream>

class vec3 {
    public:
        vec3() : e{0,0,0} {}
        vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

        float x() const { return e[0]; }
        float y() const { return e[1]; }
        float z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        float operator[](int i) const { return e[i]; }
        float& operator[](int i) { return e[i]; }

        // Returns element-wise sum of this and vec3 v
        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        // Returns itself multiplied by the scalar s
        vec3& operator*=(const float s) {
            e[0] *= s;
            e[1] *= s;
            e[2] *= s;
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
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        bool near_zero() const {
            const auto lim = 1e-8;
            return (fabsf(e[0]) < lim && fabsf(e[1]) < lim && fabsf(e[2]) < lim);
        }

        // random_sphere functions supposed to be here put in rtweekend.h
        // to avoid redefinition
    public:
        float e[3];
};

// Type aliases to avoid mix-up, since this class is used for both colors and positions.
using point3 = vec3; // 3D-point
using color = vec3; // RGB color

// vec3 Utility functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(float s, const vec3 &u) {
    return vec3(u.e[0] * s, u.e[1] * s, u.e[2] * s);
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
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
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