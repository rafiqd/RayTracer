//
// Created by Rafiq on 2018-09-03.
//

#include "Sphere.h"

bool Sphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    Vector3f oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = (b*b - a*c);
    if (discriminant > 0){
        float sqrtTemp = sqrtf(discriminant);
        float temp = (-b - sqrtTemp) / a;
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = r.point_at_t(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.matPtr = matPtr;
            return true;
        }
        temp = (-b + sqrtTemp) / a;
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = r.point_at_t(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.matPtr = matPtr;
            return true;
        }
    }

    return false;
}