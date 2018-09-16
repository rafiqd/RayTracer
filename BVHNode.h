//
// Created by Rafiq on 2018-09-09.
//

#ifndef RENDERER_BVHNODE_H
#define RENDERER_BVHNODE_H

#include "AABB.h"
#include "Hitable.h"
#include "Ray.h"

int boxXcmp(const void *, const void *);
int boxYcmp(const void *, const void *);
int boxZcmp(const void *, const void *);

class BVHNode : public Hitable {
public:
    AABB box;
    Hitable *left;
    Hitable *right;

    BVHNode() = default;
    BVHNode(Hitable **l, int n, float time0, float time1);

    bool hit(const Ray& r, float tmin, float tmax, HitRecord &rec) const override;
    bool boundingBox(float t0, float t1, AABB& box) const override;
};


#endif //RENDERER_BVHNODE_H
