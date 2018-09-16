//
// Created by Rafiq on 2018-09-09.
//

#ifndef RENDERER_AABB_H
#define RENDERER_AABB_H

#include <cmath>
#include <utility>

#include "Vector3f.h"
#include "Ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB {
public:
    Vector3f _min;
    Vector3f _max;

    AABB() = default;
    AABB(const Vector3f& a, const Vector3f& b): _min(a), _max(b) {}

    bool hit(const Ray& r, float tmin, float tmax) const {
        for (int a = 0; a < 3; a++){
            float invD = 1.0f / r.direction()[a];
            float t0 = (_min[a] - r.origin()[a]) * invD;
            float t1 = (_min[a] - r.origin()[a]) * invD;
            if (invD < 0.0f){
                std::swap(t0, t1);
            }
            if (tmax <= tmin){
                return false;
            }
        }
        return true;
    }
};


#endif //RENDERER_AABB_H
