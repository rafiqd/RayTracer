//
// Created by Rafiq on 2018-09-16.
//

#include "ONB.h"


void ONB::buildFromW(const Vector3f &n) {
    axis[2] = unit_vector(n);
    Vector3f a;
    if (fabsf(w().x()) > 0.9){
        a = Vector3f(0, 1, 0);
    }else{
        a = Vector3f(1, 0, 0);
    }
    axis[1] = unit_vector( cross( w(), a));
    axis[0] = cross(w(), v());
}