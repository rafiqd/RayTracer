//
// Created by Rafiq on 2018-09-06.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"
#include "main.h"
#include "ONB.h"
#include "PDF.h"

class ScatterRecord {
public:
    Ray SpecularRay;
    bool isSpecular;
    Vector3f attenuation;
    PDF pdfPtr;
    CosinePDF cospdf;
};

class Material {
public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const = 0;
    virtual Vector3f emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Vector3f& p) const { return Vector3f(0, 0, 0); }
    virtual float scattering_pdf(const Ray& r_in, const HitRecord& hrec, const Ray& scattered) const {
        return false;
    }
};

class DiffuseLight : public Material {
public:
    Texture *emit;

    explicit DiffuseLight(Texture *a): emit(a) {}
    bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const override {
        return false;
    }

    Vector3f emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Vector3f& p) const override {
        if (dot(rec.normal, r_in.direction()) < 0.0f)
            return emit->value(u,v,p);
        return Vector3f(0,0,0);
    }

};

class Isotropic: public Material {
public:
    Texture *albedo;
    explicit Isotropic(Texture *a): albedo(a) {}
    bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const override {
        srec.SpecularRay = Ray(rec.p, randomInUnitSphere());
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};

class Dielectric : public Material {
public:
    Dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const {
        srec.isSpecular = true;
        srec.attenuation = Vector3f(1.0, 1.0, 1.0);
        Vector3f outward_normal;
        Vector3f reflected = reflect(r_in.direction(), hrec.normal);
        Vector3f refracted;
        float ni_over_nt;
        float reflect_prob;
        float cosine;
        if (dot(r_in.direction(), hrec.normal) > 0) {
            outward_normal = -hrec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), hrec.normal) / r_in.direction().length();
        }
        else {
            outward_normal = hrec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), hrec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        }
        else {
            reflect_prob = 1.0;
        }

        float r1 = gen.UniformFloat(0,1);
        if (r1 < reflect_prob) {
            srec.SpecularRay = Ray(hrec.p, reflected);
        }
        else {
            srec.SpecularRay = Ray(hrec.p, refracted);
        }
        return true;
    }

    float ref_idx;
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
    bool scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const override {
        Vector3f reflected = reflect(unit_vector(r_in.direction()), hrec.normal);
        srec.SpecularRay = Ray(hrec.p, reflected + fuzz*randomInUnitSphere());
        srec.attenuation = albedo;
        srec.isSpecular = true;
        return true;
    }
};

class Lambertian : public Material {
public:
    Texture *albedo;

    explicit Lambertian(Texture *a): albedo(a) {};
    bool scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const override {
        srec.isSpecular = false;
        srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.p);
        srec.cospdf = CosinePDF(hrec.normal);
        return true;
    }

    float scattering_pdf(const Ray& r_in, const HitRecord& hrec, const Ray& scattered) const override {
        float cosine = dot(hrec.normal, unit_vector(scattered.direction()));
        if (cosine < 0)
            return 0;
        return cosine / M_PI;
    }
};

#endif //RENDERER_MATERIAL_H
