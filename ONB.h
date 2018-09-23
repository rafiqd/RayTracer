//
// Created by Rafiq on 2018-09-16.
//

#ifndef RENDERER_ONB_H
#define RENDERER_ONB_H

#include "Vector3f.h"

class ONB {
public:
    Vector3f axis[3];

    ONB() = default;
    Vector3f u() const { return axis[0]; }
    Vector3f v() const { return axis[1]; }
    Vector3f w() const { return axis[2]; }
    Vector3f local(float a, float b, float c) const { return a*u() + b*v() + c*w(); }
    Vector3f local(const Vector3f& a) const { return a.x()*u() + a.y()*v() + a.z()*w(); }
    void buildFromW(const Vector3f&);
    inline Vector3f operator[](int i) const { return axis[i]; }
};


#endif //RENDERER_ONB_H
