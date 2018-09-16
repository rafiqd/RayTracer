//
// Created by Rafiq on 2018-09-03.
//

#include <cfloat>
#include "Hitable.h"

AABB Hitable::surroundingBox(AABB box0, AABB box1) const {
    Vector3f small( fminf(box0._min.x(), box1._min.x()),
                    fminf(box0._min.y(), box1._min.y()),
                    fminf(box0._min.z(), box1._min.z())
    );
    Vector3f big  ( fmaxf(box0._max.x(), box1._max.x()),
                    fmaxf(box0._max.y(), box1._max.y()),
                    fmaxf(box0._max.z(), box1._max.z())
    );
    return AABB(small, big);
}

RotateY::RotateY(Hitable *p, float angle): ptr(p) {
    float radians = (M_PI / 180.0f) * angle;
    sin_theta = sinf(radians);
    cos_theta = cosf(radians);
    hasbox = ptr->boundingBox(0, 1, bbox);
    Vector3f min(FLT_MAX, FLT_MAX, FLT_MAX);
    Vector3f max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < 2; k++){
                float x = i*bbox._max.x() + (1-i)*bbox._min.x();
                float y = j*bbox._max.y() + (1-j)*bbox._min.y();
                float z = k*bbox._max.z() + (1-k)*bbox._min.z();

                float newx = cos_theta * x + sin_theta * z;
                float newz = -sin_theta * x + cos_theta * z;

                Vector3f tester(newx, y, newz);
                for (int c = 0; c < 3; c++ ){
                    if ( tester[c] > max[c] ){
                        max[c] = tester[c];
                    }
                    if ( tester[c] < min[c] ){
                        min[c] = tester[c];
                    }
                }
            }
        }
    }
    bbox = AABB(min, max);
}

bool RotateY::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
    Vector3f origin = r.origin();
    Vector3f direction = r.direction();
    origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];

    direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

    Ray rotated_r(origin, direction, r.time);
    if ( ptr->hit(rotated_r, tMin, tMax, rec)){
        Vector3f p = rec.p;
        Vector3f normal = rec.normal;
        p[0] = cos_theta*rec.p[0] + sin_theta*rec.p[2];
        p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];
        normal[0] = cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
        normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    return false;
}