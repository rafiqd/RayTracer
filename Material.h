//
// Created by Rafiq on 2018-09-06.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"
#include "main.h"

Vector3f randomInUnitSphere();

class Material {
public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3f& attenuation, Ray& scattered) const = 0;
    virtual Vector3f emitted(float u, float v, const Vector3f& p) const { return Vector3f(0, 0, 0); }
};

class DiffuseLight : public Material {
public:
    Texture *emit;

    explicit DiffuseLight(Texture *a): emit(a) {}
    bool scatter(const Ray& r_in, const HitRecord& rec, Vector3f& attenuation, Ray& scattered) const override {
        return false;
    }

    Vector3f emitted(float u, float v, const Vector3f& p) const override {
        return emit->value(u, v, p);
    }

};

class Isotropic: public Material {
public:
    Texture *albedo;
    explicit Isotropic(Texture *a): albedo(a) {}
    bool scatter(const Ray& r_in, const HitRecord& rec, Vector3f& attenuation, Ray& scattered) const override {
        scattered = Ray(rec.p, randomInUnitSphere());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};

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

#endif //RENDERER_MATERIAL_H
