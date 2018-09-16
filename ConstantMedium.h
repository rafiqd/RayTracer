//
// Created by Rafiq on 2018-09-15.
//

#ifndef RENDERER_CONSTANTMEDIUM_H
#define RENDERER_CONSTANTMEDIUM_H

#include "Hitable.h"
#include "Material.h"

class ConstantMedium : public Hitable {
public:
    Hitable *boundary;
    float density;
    Material *phaseFunction;

    ConstantMedium(Hitable *b, float d, Texture *a): boundary(b), density(d){
        phaseFunction = new Isotropic(a);
    }
    bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override;
    bool boundingBox(float t0, float t1, AABB &box) const override {
        return boundary->boundingBox(t0, t1, box);
    }
};


#endif //RENDERER_CONSTANTMEDIUM_H
