//
// Created by Rafiq on 2018-09-13.
//

#ifndef RENDERER_MAIN_H
#define RENDERER_MAIN_H

#include <thread>

#include "Vector3f.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Camera.h"
#include "Triangle.h"
#include "Light.h"

Vector3f color(const Ray& r, Hitable *world, Hitable *lightShape, int depth);
bool refract(const Vector3f& v, const Vector3f& n, float niOverNt, Vector3f& refracted);
float schlick(float cosine, float refIdx);
Vector3f randomInUnitDisk();
Vector3f randomInUnitSphere();
std::string render();
Vector3f reflect(const Vector3f& v, const Vector3f& n);
inline Vector3f randomCosineDireciton(){
    float r1 = drand48();
    float r2 = drand48();
    float z = sqrtf(1-r2);
    float phi = 2*M_PI*r1;
    float x = cos(phi) * 2 * sqrt(r2);
    float y = sin(phi) * 2 * sqrt(r2);
    return Vector3f(x, y , z);
}

inline Vector3f deNan(const Vector3f& c){
    Vector3f temp = c;
    if (!(temp[0] == temp[0])) temp[0] = 0;
    if (!(temp[1] == temp[1])) temp[1] = 0;
    if (!(temp[2] == temp[2])) temp[2] = 0;
    return temp;
}
extern Vector3f *output_img;
extern Camera *render_cam;
extern Hitable *world;
extern HitableList hlist;
extern int *workedon;
extern int nx;
extern int ny;
extern int ns;
#endif //RENDERER_MAIN_H
