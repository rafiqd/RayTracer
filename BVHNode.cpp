//
// Created by Rafiq on 2018-09-09.
//

#include "BVHNode.h"

bool BVHNode::boundingBox(float t0, float t1, AABB &b) const {
    b = box;
    return true;

}

bool BVHNode::hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const {
    if(box.hit(r, tmin, tmax)){
        HitRecord leftRec, rightRec;
        bool hitLeft = left->hit(r, tmin, tmax, leftRec);
        bool hitRight = right->hit(r, tmin, tmax, rightRec);

        if (hitLeft && hitRight){
            if (leftRec.t < rightRec.t){
                rec = leftRec;
            }else {
                rec = rightRec;
            }
        }else if( hitLeft ){
            rec = leftRec;
            return true;
        }else if( hitRight ){
            rec = rightRec;
            return true;
        } else
            return false;
    }else
        return false;
}

BVHNode::BVHNode(Hitable **l, int n, float time0, float time1) {
    int axis = int(3 * Distributions::zero_to_one.getRandFloat() );
    if (axis == 0)
        qsort(l, n, sizeof(Hitable*), boxXcmp);
    else if (axis == 1)
        qsort(l, n, sizeof(Hitable*), boxYcmp);
    else
        qsort(l, n, sizeof(Hitable*), boxZcmp);

    if (n == 1){
        left = right = l[0];
    }else{
        left = new BVHNode(l, n/2, time0, time1);
        right = new BVHNode(l + n/2, n - n/2, time0, time1);
    }
    AABB boxLeft, boxRight;
    if(!left->boundingBox(time0, time1, boxLeft) || !right->boundingBox(time0, time1, boxRight))
        std::cerr << "no bounding box in BVHNode constructor\n";
    box = surroundingBox(boxLeft, boxRight);
}

int boxXcmp(const void *a, const void *b){
    AABB boxLeft, boxRight;
    Hitable *ah = *(Hitable**)a;
    Hitable *bh = *(Hitable**)b;
    if(!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        std::cerr << "no bounding box in BVHNode constructor\n";
    if ( boxLeft._min.x() - boxRight._min.x() < 0.0 )
        return -1;
    else
        return 1;
}

int boxYcmp(const void *a, const void *b){
    AABB boxLeft, boxRight;
    Hitable *ah = *(Hitable**)a;
    Hitable *bh = *(Hitable**)b;
    if(!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        std::cerr << "no bounding box in BVHNode constructor\n";
    if ( boxLeft._min.y() - boxRight._min.y() < 0.0 )
        return -1;
    else
        return 1;
}

int boxZcmp(const void *a, const void *b){
    AABB boxLeft, boxRight;
    Hitable *ah = *(Hitable**)a;
    Hitable *bh = *(Hitable**)b;
    if(!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        std::cerr << "no bounding box in BVHNode constructor\n";
    if ( boxLeft._min.z() - boxRight._min.z() < 0.0 )
        return -1;
    else
        return 1;
}