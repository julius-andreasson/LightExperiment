#pragma once

#include "vec3.h"

class ray {
    public:
        ray() {}
        ray(const point3& origin, const vec3& direction) 
            : orig(origin), dir(direction)
        {}

        point3 origin() const { return orig; }
        vec3 direction() const { return dir; }

        point3 at(float s) const {
            return orig + s * dir;
        }

    public:
        point3 orig;
        vec3 dir;
};