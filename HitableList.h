//
// Created by Rafiq on 2018-09-03.
//

#ifndef RENDERER_HITABLELIST_H
#define RENDERER_HITABLELIST_H

#include "RNG.h"
#include "Hitable.h"
#include "AABB.h"


class HitableList : public Hitable {
public:
    Hitable **list;
    int listSize;

    HitableList() = default;
    HitableList(Hitable **l, int n) { list = l; listSize = n; }

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override ;
    bool boundingBox(float t0, float t1, AABB& box) const override;
    float pdfValue(const Vector3f& o, const Vector3f& v) const override {
        float weight = 1.0f / listSize;
        float sum = 0;
        for (int i = 0; i < listSize; i++){
            sum += weight * list[i]->pdfValue(o, v);
        }
        return sum;
    }

    Vector3f random(const Vector3f& o) const override {
        float r1 = gen.UniformFloat(0,1);
        int index = int(r1 * listSize);
        return list[ index ]->random(o);
    }

};




#endif //RENDERER_HITABLELIST_H
