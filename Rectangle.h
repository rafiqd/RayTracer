//
// Created by Rafiq on 2018-09-13.
//

#ifndef RENDERER_RECTANGLE_H
#define RENDERER_RECTANGLE_H

#include "Hitable.h"
#include "Ray.h"
#include "AABB.h"

class Material;

class XYRectangle: public Hitable {
public:
    float x0, x1;
    float y0, y1;
    float k;
    Material *mp;

    XYRectangle() = default;
    XYRectangle(float _x0, float _x1, float _y0, float _y1, float _k, Material *mat):
            x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override ;
    bool boundingBox(float t0, float t1, AABB& box) const override {
        box = AABB(Vector3f(x0, y0, k - 0.0001f), Vector3f(x1, y1, k + 0.0001f));
        return true;
    }

};

class XZRectangle : public Hitable {
public:
    float x0, x1;
    float z0, z1;
    float k;
    Material *mp;

    XZRectangle() = default;
    XZRectangle(float _x0, float _x1, float _z0, float _z1, float _k, Material *mat):
            x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override ;
    bool boundingBox(float t0, float t1, AABB& box) const override {
        box = AABB(Vector3f(x0, k - 0.0001f, z0), Vector3f(x1, k + 0.0001f, z1));
        return true;
    }
    float pdfValue(const Vector3f& o, const Vector3f& v) const override;
    Vector3f random(const Vector3f& o) const override;
};

class YZRectangle : public Hitable {
public:
    float y0, y1;
    float z0, z1;
    float k;
    Material *mp;

    YZRectangle() = default;
    YZRectangle(float _y0, float _y1, float _z0, float _z1, float _k, Material *mat):
            y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override ;
    bool boundingBox(float t0, float t1, AABB& box) const override {
        box = AABB(Vector3f(k - 0.0001f, y0, z0), Vector3f(k + 0.0001f, y1, z1));
        return true;
    }
};

class Box : public Hitable {
public:
    Vector3f pmin, pmax;
    Hitable *listPtr;
    Box() = default;
    Box(const Vector3f& p0, const Vector3f& p1, Material *ptr);
    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override ;
    bool boundingBox(float t0, float t1, AABB& box) const override {
        box = AABB(pmin, pmax);
        return true;
    }
};





#endif //RENDERER_RECTANGLE_H
