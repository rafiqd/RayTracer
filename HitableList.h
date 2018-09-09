//
// Created by Rafiq on 2018-09-03.
//

#ifndef RENDERER_HITABLELIST_H
#define RENDERER_HITABLELIST_H

#include "Hitable.h"


class HitableList : public Hitable {
public:
    Hitable **list;
    int listSize;

    HitableList() = default;
    HitableList(Hitable **l, int n) { list = l; listSize = n; }

    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
};



#endif //RENDERER_HITABLELIST_H
