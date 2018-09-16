//
// Created by Rafiq on 2018-09-06.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"

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

#endif //RENDERER_MATERIAL_H
