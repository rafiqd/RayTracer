//
// Created by Rafiq on 2018-09-09.
//

#ifndef RENDERER_AABB_H
#define RENDERER_AABB_H

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

    Vector3f min() const { return _min; }
    Vector3f max() const { return _max; }

    bool hit(const Ray& r, float tmin, float tmax) const {
        for (int a = 0; a < 3; a++) {
            float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a],
                             (_max[a] - r.origin()[a]) / r.direction()[a]);
            float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a],
                             (_max[a] - r.origin()[a]) / r.direction()[a]);
            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin)
                return false;
        }
        return true;
    }

    float area() const {
        float a = _max.x() - _min.x();
        float b = _max.y() - _min.y();
        float c = _max.z() - _min.z();
        return 2*(a*b + b*c + c*a);
    }

    int longest_axis() const {
        float a = _max.x() - _min.x();
        float b = _max.y() - _min.y();
        float c = _max.z() - _min.z();
        if (a > b && a > c)
            return 0;
        else if (b > c)
            return 1;
        else
            return 2;
    }


};

//AABB surrounding_box(AABB box0, AABB box1) {
//    Vector3f small( fmin(box0.min().x(), box1.min().x()),
//                fmin(box0.min().y(), box1.min().y()),
//                fmin(box0.min().z(), box1.min().z()));
//    Vector3f big  ( fmax(box0.max().x(), box1.max().x()),
//                fmax(box0.max().y(), box1.max().y()),
//                fmax(box0.max().z(), box1.max().z()));
//    return AABB(small,big);
//}

#endif //RENDERER_AABB_H
