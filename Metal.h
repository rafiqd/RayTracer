//
// Created by Rafiq on 2018-09-06.
//

#ifndef RENDERER_METAL_H
#define RENDERER_METAL_H

#include "Vector3f.h"
#include "Ray.h"
#include "Material.h"

Vector3f reflect(const Vector3f&, const Vector3f&);

class Metal : public Material {
public:
    Vector3f albedo;
    float fuzz;

    Metal(const Vector3f& a, float f): albedo(a){
        if (f < 1){
            fuzz = f;
        }else{
            fuzz = 1;
        }
    }
    bool scatter(const Ray& r_in, const HitRecord& rec, Vector3f& attenuation, Ray& scattered) const override {
        Vector3f reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }
};


#endif //RENDERER_METAL_H
