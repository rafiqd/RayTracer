//
// Created by Rafiq on 2018-09-03.
//

#include "HitableList.h"


bool HitableList::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {

    bool hitAnything = false;
    float closestSoFar = tMax;
    HitRecord tempRec;

    for(int i = 0; i < listSize; ++i){
        if(list[i]->hit(r, tMin, closestSoFar, tempRec)){
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }
    return hitAnything;
}