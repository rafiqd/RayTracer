//
// Created by Rafiq on 2018-09-09.
//

#ifndef RENDERER_DIELECTRIC_H
#define RENDERER_DIELECTRIC_H


#include <functional>
#include <random>

#include "Material.h"
#include "Vector3f.h"
#include "cstdlib"

Vector3f reflect(const Vector3f&, const Vector3f&);
bool refract(const Vector3f&, const Vector3f&, float, Vector3f&);
float schlick(float, float );


class Dielectric : public Material {
public:
    float refIdx;

    Dielectric(float ri): refIdx(ri) {
    }

    bool scatter(const Ray& r_in, const HitRecord& rec, Vector3f& attenuation, Ray& scattered) const override {
        Vector3f outwardNormal;
        Vector3f reflected = reflect(r_in.direction(), rec.normal);
        float niOverNt;
        attenuation = Vector3f(1.0, 1.0, 1.0);
        Vector3f refracted;
        float cosine;
        float reflectProb;
        if (dot(r_in.direction(), rec.normal) > 0){
            outwardNormal = -rec.normal;
            niOverNt = refIdx;
            cosine = refIdx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        } else {
            outwardNormal = rec.normal;
            niOverNt = 1.0f / refIdx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        if (refract(r_in.direction(), outwardNormal, niOverNt, refracted)){
            reflectProb = schlick(cosine, refIdx);
        }else{
            reflectProb = 1.0;
        }
        float random = float(rand()) / float(RAND_MAX);

        if(random < reflectProb){
            scattered = Ray(rec.p, reflected);
        } else {
            scattered = Ray(rec.p, refracted);
        }
        return true;
    }
};


#endif //RENDERER_DIELECTRIC_H
