//
// Created by Rafiq on 2018-09-03.
//

#ifndef RENDERER_HITABLE_H
#define RENDERER_HITABLE_H

#include "Vector3f.h"
#include "Ray.h"

class Material;

class HitRecord {
public:
    float t;
    Vector3f p;
    Vector3f normal;
    Material *matPtr;
};


class Hitable {
public:
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};

#endif //RENDERER_HITABLE_H
