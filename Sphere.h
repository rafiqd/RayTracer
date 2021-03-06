//
// Created by Rafiq on 2018-09-03.
//

#ifndef RENDERER_SPHERE_H
#define RENDERER_SPHERE_H

#include "Hitable.h"
#include "Material.h"

void getSphereUV(const Vector3f& p, float& u, float& v);

inline Vector3f randomToSphere(float radius, float distanceSquared){
    float r1 = gen.UniformFloat(0,1);
    float r2 = gen.UniformFloat(0,1);
    float z = 1 + r2 * (sqrtf(1 - radius*radius/distanceSquared) - 1);
    float phi = 2*M_PI*r1;
    float x = cosf(phi)*sqrtf(1-z*z);
    float y = sinf(phi)*sqrtf(1-z*z);
    return Vector3f(x, y , z);
}

class Sphere: public Hitable {
public:
    Vector3f center;
    float radius;
    Material *matPtr;

    Sphere() = default;
    Sphere(Vector3f cen, float r, Material* mtl): center(cen), radius(r), matPtr(mtl) {};

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override ;
    bool boundingBox(float t0, float t1, AABB& box) const override ;
    float pdfValue(const Vector3f& o, const Vector3f& v) const override ;
    Vector3f random(const Vector3f& o) const override ;

};

class MovingSphere : public Hitable {
public:
    Vector3f center0, center1;
    float time0, time1;
    float radius;
    Material *matPtr;

    Vector3f center(float time) const;


    MovingSphere() {}
    MovingSphere(Vector3f cen0, Vector3f cen1, float t0, float t1, float r, Material *m):
            center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), matPtr(m)  {};
    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override ;
    bool boundingBox(float t0, float t1, AABB& box) const override ;
    AABB surroundingBox(AABB box0, AABB box1) const;
};

#endif //RENDERER_SPHERE_H
