//
// Created by Rafiq on 2018-09-06.
//

#ifndef RENDERER_LAMBERTIAN_H
#define RENDERER_LAMBERTIAN_H

#include "Material.h"
#include "Vector3f.h"
#include "Ray.h"
#include "Texture.h"

Vector3f randomInUnitSphere();

class Lambertian : public Material {
public:
     Texture *albedo;

    explicit Lambertian(Texture *a): albedo(a) {};
    bool scatter(const Ray& r_in, const HitRecord& rec, Vector3f& attenuation, Ray& scattered) const override {
        Vector3f target = rec.p + rec.normal + randomInUnitSphere();
        scattered = Ray(rec.p, target-rec.p, r_in.time);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};


#endif //RENDERER_LAMBERTIAN_H
