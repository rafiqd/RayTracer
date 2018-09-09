//
// Created by Rafiq on 2018-09-02.
//

#ifndef RENDERER_RAY_H
#define RENDERER_RAY_H

#include <ostream>

#include "Vector3f.h"

class Ray {
public:
    Vector3f o;
    Vector3f d;
    Ray() = default;
    Ray(const Vector3f& org, const Vector3f dest): o(org), d(dest){}

    Vector3f origin() const { return o; }
    Vector3f direction() const { return d; }
    Vector3f point_at_t(float t) const { return o + t*d; }

    friend std::ostream& operator<<(std::ostream &os, const Ray r){
        return os << "Ray(" << r.o << ", " << r.d << ")";
    }

};


#endif //RENDERER_RAY_H
