//
// Created by Rafiq on 2018-09-03.
//

#ifndef RENDERER_SPHERE_H
#define RENDERER_SPHERE_H

#include "Hitable.h"
#include "Material.h"

class Sphere: public Hitable {
public:
    Vector3f center;
    float radius;
    Material *matPtr;

    Sphere() = default;
    Sphere(Vector3f cen, float r, Material* mtl): center(cen), radius(r), matPtr(mtl) {};

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override ;
};

#endif //RENDERER_SPHERE_H
