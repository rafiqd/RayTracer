//
// Created by Rafiq on 2018-09-09.
//

#include <cfloat>
#include "BVHNode.h"

bool BVHNode::boundingBox(float t0, float t1, AABB &b) const {
    b = box;
    return true;

}

bool BVHNode::hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const {
    if (box.hit(r, tmin, tmax)) {
        HitRecord left_rec, right_rec;
        bool hit_left = left->hit(r, tmin, tmax, left_rec);
        bool hit_right = right->hit(r, tmin, tmax, right_rec);
        if (hit_left && hit_right) {
            if (left_rec.t < right_rec.t)
                rec = left_rec;
            else
                rec = right_rec;
            return true;
        }
        else if (hit_left) {
            rec = left_rec;
            return true;
        }
        else if (hit_right) {
            rec = right_rec;
            return true;
        }
        else
            return false;
    }
    else return false;
}

BVHNode::BVHNode(Hitable **l, int n, float time0, float time1) {
    AABB *boxes = new AABB[n];
    float *left_area = new float[n];
    float *right_area = new float[n];
    AABB main_box;
    bool dummy = l[0]->boundingBox(time0,time1,main_box);
    for (int i = 1; i < n; i++) {
        AABB new_box;
        bool dummy = l[i]->boundingBox(time0,time1,new_box);
        main_box = surroundingBox(new_box, main_box);
    }
    int axis = main_box.longest_axis();
    if (axis == 0)
        qsort(l, n, sizeof(Hitable *), boxXcmp);
    else if (axis == 1)
        qsort(l, n, sizeof(Hitable *), boxYcmp);
    else
        qsort(l, n, sizeof(Hitable *), boxZcmp);
    for (int i = 0; i < n; i++)
        bool dummy = l[i]->boundingBox(time0,time1,boxes[i]);
    left_area[0] = boxes[0].area();
    AABB left_box = boxes[0];
    for (int i = 1; i < n-1; i++) {
        left_box = surroundingBox(left_box, boxes[i]);
        left_area[i] = left_box.area();
    }
    right_area[n-1] = boxes[n-1].area();
    AABB right_box = boxes[n-1];
    for (int i = n-2; i > 0; i--) {
        right_box = surroundingBox(right_box, boxes[i]);
        right_area[i] = right_box.area();
    }
    float min_SAH = FLT_MAX;
    int min_SAH_idx;
    for (int i = 0; i < n-1; i++) {
        float SAH = i*left_area[i] + (n-i-1)*right_area[i+1];
        if (SAH < min_SAH) {
            min_SAH_idx = i;
            min_SAH = SAH;
        }
    }

    if (n == 1) {
        left = right = l[0];
    }
    else if (n == 2) {
        left = l[0];
        right = l[1];
    }
    else {
        left = new BVHNode(l, n/2, time0, time1);
        right = new BVHNode(l + n/2, n - n/2, time0, time1);
    }

    box = main_box;
}

int boxXcmp(const void *a, const void *b){
    AABB boxLeft, boxRight;
    Hitable *ah = *(Hitable**)a;
    Hitable *bh = *(Hitable**)b;
    if(!ah->boundingBox(0,0, boxLeft) || !bh->boundingBox(0,0, boxRight))
        std::cerr << "no bounding box in BVHNode constructor\n";
    if ( boxLeft.min().x() - boxRight.min().x() < 0.0  )
        return -1;
    else
        return 1;
}

int boxYcmp(const void *a, const void *b){

    AABB box_left, box_right;
    Hitable *ah = *(Hitable**)a;
    Hitable *bh = *(Hitable**)b;
    if(!ah->boundingBox(0,0, box_left) || !bh->boundingBox(0,0, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";
    if ( box_left.min().y() - box_right.min().y() < 0.0  )
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