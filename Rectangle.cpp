//
// Created by Rafiq on 2018-09-13.
//

#include <cfloat>
#include "HitableList.h"
#include "Rectangle.h"

Box::Box(const Vector3f& p0, const Vector3f& p1, Material *ptr){
    pmin = p0;
    pmax = p1;
    Hitable **list = new Hitable*[6];
    list[0] = new XYRectangle(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
    list[1] = new FlipNormals(new XYRectangle(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
    list[2] = new XZRectangle(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
    list[3] = new FlipNormals(new XZRectangle(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
    list[4] = new YZRectangle(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
    list[5] = new FlipNormals(new YZRectangle(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
    listPtr = new HitableList(list, 6);
}


bool XYRectangle::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
    float t = (k-r.origin().z()) / r.direction().z();
    if ( t < t0 || t > t1 )
        return false;
    float x = r.origin().x() + t * r.direction().x();
    float y = r.origin().y() + t * r.direction().y();
    if ( x < x0 || x > x1 || y < y0 || y > y1 )
        return false;
    rec.u = (x-x0) / (x1-x0);
    rec.v = (y-y0) / (y1-y0);
    rec.t = t;
    rec.matPtr = mp;
    rec.p = r.point_at_t(t);
    rec.normal = Vector3f(0, 0, 1);
    return true;
}

bool XZRectangle::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.origin().y()) / r.direction().y();
    if ( t < t0 || t > t1 )
        return false;
    float x = r.origin().x() + t * r.direction().x();
    float z = r.origin().z() + t * r.direction().z();
    if ( x < x0 || x > x1 || z < z0 || z > z1 )
        return false;
    rec.u = (x-x0) / (x1-x0);
    rec.v = (z-z0) / (z1-z0);
    rec.t = t;
    rec.matPtr = mp;
    rec.p = r.point_at_t(t);
    rec.normal = Vector3f(0, 1, 0);
    return true;
}

bool YZRectangle::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.origin().x()) / r.direction().x();
    if ( t < t0 || t > t1 )
        return false;
    float y = r.origin().y() + t * r.direction().y();
    float z = r.origin().z() + t * r.direction().z();
    if ( z < z0 || z > z1 || y < y0 || y > y1 )
        return false;
    rec.u = (z-z0) / (z1-z0);
    rec.v = (y-y0) / (y1-y0);
    rec.t = t;
    rec.matPtr = mp;
    rec.p = r.point_at_t(t);
    rec.normal = Vector3f(1, 0, 0);
    return true;
}


bool Box::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    return listPtr->hit(r, tMin, tMax, rec);
}

float XZRectangle::pdfValue(const Vector3f& o, const Vector3f& v) const {
    HitRecord rec;
    if (this->hit(Ray(o,v), 0.001, FLT_MAX, rec)){
        float area = (x1 - x0) * (z1 - z0);
        float distance_squared = rec.t * rec.t * v.squared_length();
        float cosine = fabsf(dot(v, rec.normal) / v.length());
        return distance_squared / (cosine * area);
    }
    return 0;
}

Vector3f XZRectangle::random(const Vector3f& o) const {
    float r1 = gen.UniformFloat(0,1);
    float r2 = gen.UniformFloat(0,1);
    Vector3f randomPoint = Vector3f(x0 + r1*(x1-x0), k, z0 + r2*(z1-z0));
    return randomPoint - o;
}