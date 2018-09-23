//
// Created by Rafiq on 2018-09-03.
//

#include "HitableList.h"


bool HitableList::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;
    for (int i = 0; i < listSize; i++) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool HitableList::boundingBox(float t0, float t1, AABB& box) const {
    if (listSize < 1) return false;
    AABB tempBox;
    bool firstTrue = list[0]->boundingBox(t0, t1, tempBox);
    if ( !firstTrue )
        return false;
    else
        box = tempBox;
    for (int i = 1; i < listSize; i++){
        if(list[0]->boundingBox(t0, t1, tempBox)){
            box = surroundingBox(box, tempBox);
        } else
            return false;
    }
    return true;
}
