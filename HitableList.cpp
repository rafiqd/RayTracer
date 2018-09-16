//
// Created by Rafiq on 2018-09-03.
//

#include "HitableList.h"


//AABB HitableList::surroundingBox(AABB box0, AABB box1) const {
//    Vector3f small( fminf(box0._min.x(), box1._min.x()),
//                    fminf(box0._min.y(), box1._min.y()),
//                    fminf(box0._min.z(), box1._min.z())
//    );
//    Vector3f big  ( fmaxf(box0._max.x(), box1._max.x()),
//                    fmaxf(box0._max.y(), box1._max.y()),
//                    fmaxf(box0._max.z(), box1._max.z())
//    );
//    return AABB(small, big);
//}