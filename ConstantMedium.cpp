//
// Created by Rafiq on 2018-09-15.
//

#include <cfloat>
#include "ConstantMedium.h"

bool ConstantMedium::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
    bool db = drand48() < 0.00001;
    db = false;
    HitRecord rec1, rec2;
    if( boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)){
        if (boundary->hit(r, rec1.t + 0.0001f, FLT_MAX, rec2)){
            if (db){
                std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << "\n";
            }

            if (rec1.t < tMin)
                rec1.t = tMin;
            if (rec2.t > tMax)
                rec2.t = tMax;
            if (rec1.t >= rec2.t)
                return false;
            if (rec1.t < 0)
                rec1.t = 0;
            float distanceInsideBoundary = (rec2.t - rec1.t) * r.direction().length();
            float hitDistance = -(1/density) * logf(drand48());
            if ( hitDistance < distanceInsideBoundary ) {
                if (db) std::cerr << "hit_distance = " <<  hitDistance << "\n";
                rec.t = rec1.t + hitDistance / r.direction().length();
                if (db) std::cerr << "rec.t = " <<  rec.t << "\n";
                rec.p = r.point_at_t(rec.t);
                if (db) std::cerr << "rec.p = " <<  rec.p << "\n";
                rec.normal = Vector3f(1,0,0);  // arbitrary
                rec.matPtr = phaseFunction;
                return true;
            }
        }
    }
    return false;
}