//
// Created by Rafiq on 2018-09-03.
//

#include "Sphere.h"

void getSphereUV(const Vector3f& p, float& u, float& v){
    float phi = atan2(p.z(), p.x());
    float theta = asin(p.y());
    u = 1 - (phi + M_PI) / (2*M_PI);
    v = (theta + M_PI/2) / M_PI;
}

bool Sphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    Vector3f oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0){
        float temp = (-b - sqrt(b*b-a*c))/a;
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = r.point_at_t(rec.t);
            getSphereUV((rec.p - center)/radius, rec.u, rec.v);
            rec.normal = (rec.p - center) / radius;
            rec.matPtr = matPtr;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = r.point_at_t(rec.t);
            getSphereUV((rec.p - center)/radius, rec.u, rec.v);
            rec.normal = (rec.p - center) / radius;
            rec.matPtr = matPtr;
            return true;
        }
    }

    return false;
}

bool Sphere::boundingBox(float t0, float t1, AABB& box) const {
    box = AABB(center - Vector3f(radius, radius, radius), center + Vector3f(radius, radius, radius));
    return true;
}

Vector3f MovingSphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}


bool MovingSphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    Vector3f oc = r.origin() - center(r.time);
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
            rec.normal = (rec.p - center(r.time)) / radius;
            getSphereUV((rec.p - center(r.time))/radius, rec.u, rec.v);
            rec.matPtr = matPtr;
            return true;
        }
        temp = (-b + sqrtTemp) / a;
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = r.point_at_t(rec.t);
            rec.normal = (rec.p - center(r.time)) / radius;
            getSphereUV((rec.p - center(r.time))/radius, rec.u, rec.v);
            rec.matPtr = matPtr;
            return true;
        }
    }

    return false;
}

bool MovingSphere::boundingBox(float t0, float t1, AABB& box) const {
    AABB box0(center(t0) - Vector3f(radius, radius, radius), center(t0) + Vector3f(radius, radius, radius));
    AABB box1(center(t1) - Vector3f(radius, radius, radius), center(t1) + Vector3f(radius, radius, radius));
    box = surroundingBox(box0, box1);
    return true;

}

AABB MovingSphere::surroundingBox(AABB box0, AABB box1) const {
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