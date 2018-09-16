//
// Created by Rafiq on 2018-09-03.
//

#ifndef RENDERER_HITABLE_H
#define RENDERER_HITABLE_H

#include "Vector3f.h"
#include "Ray.h"
#include "AABB.h"
#include "Texture.h"

class Material;

class HitRecord {
public:
    float t, u, v;
    Vector3f p;
    Vector3f normal;
    Material *matPtr;

};

class Hitable {
public:
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
    virtual bool boundingBox(float t0, float t1, AABB& box) const = 0;
    AABB surroundingBox(AABB box0, AABB box1) const;
};


class FlipNormals : public Hitable {
public:
    Hitable *ptr;

    explicit FlipNormals(Hitable *p): ptr(p) {};

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override {
        if (ptr->hit(r, tMin, tMax, rec)){
            rec.normal = -rec.normal;
            return true;
        } else
            return false;
    }

    bool boundingBox(float t0, float t1, AABB& box) const override {
        return ptr->boundingBox(t0, t1, box);
    }


};

class Translate : public Hitable {
public:
    Hitable *ptr;
    Vector3f offset;

    Translate(Hitable* p, const Vector3f& displacement): ptr(p), offset(displacement){}
    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override {
        Ray movedR(r.origin() - offset, r.direction(), r.time);
        if (ptr->hit(movedR, tMin, tMax, rec)){
            rec.p += offset;
            return true;
        }
        return false;
    }

    bool boundingBox(float t0, float t1, AABB& box) const override {
        if (ptr->boundingBox(t0, t1, box)){
            box = AABB(box._min + offset, box._max + offset);
            return true;
        }
        return false;
    }
};

class RotateY: public Hitable {
public:
    Hitable *ptr;
    float sin_theta;
    float cos_theta;
    AABB bbox;
    bool hasbox;

    RotateY(Hitable *p, float angle);

    bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override;

    bool boundingBox(float t0, float t1, AABB &box) const override {
        box = bbox;
        return hasbox;
    }
};


#endif //RENDERER_HITABLE_H
