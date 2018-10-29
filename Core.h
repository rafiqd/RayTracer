//
// Created by Rafiq on 2018-10-28.
//

#ifndef RENDERER_CORE_H
#define RENDERER_CORE_H

#include "Vector3f.h"
#include "Ray.h"
#include "Hitable.h"
#include "RNG.h"

inline Vector3f deNan(const Vector3f& c){
    Vector3f temp = c;
    if (!(temp[0] == temp[0])) temp[0] = 0;
    if (!(temp[1] == temp[1])) temp[1] = 0;
    if (!(temp[2] == temp[2])) temp[2] = 0;
    return temp;
}

inline Vector3f reflect(const Vector3f& v, const Vector3f& n){
    return v - 2*dot(v,n)*n;
}

bool refract(const Vector3f& v, const Vector3f& n, float niOverNt, Vector3f& refracted);

inline float schlick(const float cosine, const float refIdx){
    float r0 = (1-refIdx) / (1+refIdx);
    r0 = r0*r0;
    return r0 + (1-r0)*powf((1 - cosine), 5);
}

inline Vector3f randomInUnitDisk(){

    Vector3f p;
    do {
        p = 2.0*Vector3f(gen.UniformFloat(0,1),gen.UniformFloat(0,1),0) - Vector3f(1,1,0);
    } while (dot(p,p) >= 1.0);

    return p;
}

inline Vector3f randomInUnitSphere(){

    Vector3f p;

    do {
        float r1 = gen.UniformFloat(0,1);
        float r2 = gen.UniformFloat(0,1);
        float r3 = gen.UniformFloat(0,1);
        p = 2.0f * Vector3f(r1, r2, r3) - Vector3f(1.0, 1.0, 1.0);
    } while (dot(p,p) >= 1.0f);

    return unit_vector(p);
}

inline Vector3f randomCosineDireciton(){
    const float r1 = gen.UniformFloat(0,1);
    const float r2 = gen.UniformFloat(0,1);
    const float z = sqrtf(1-r2);
    const float phi = 2*M_PI*r1;
    const float x = cosf(phi) * 2 * sqrtf(r2);
    const float y = sinf(phi) * 2 * sqrtf(r2);
    return Vector3f(x, y , z);
}

Vector3f color(const Ray&, Hitable*, Hitable*, int&);

float calculate_var(const float data[], int num);


#endif //RENDERER_CORE_H
