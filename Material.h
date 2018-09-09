//
// Created by Rafiq on 2018-09-06.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "Ray.h"
#include "Hitable.h"

class Material {
public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3f& attenuation, Ray& scattered) const = 0;
};


#endif //RENDERER_MATERIAL_H
