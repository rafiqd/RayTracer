//
// Created by Rafiq on 2018-09-03.
//

#ifndef RENDERER_HITABLELIST_H
#define RENDERER_HITABLELIST_H

#include "Hitable.h"
#include "AABB.h"


class HitableList : public Hitable {
public:
    Hitable **list;
    int listSize;

    HitableList() = default;
    HitableList(Hitable **l, int n) { list = l; listSize = n; }

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
    bool boundingBox(float t0, float t1, AABB& box) const override;
};




#endif //RENDERER_HITABLELIST_H
