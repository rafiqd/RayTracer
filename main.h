//
// Created by Rafiq on 2018-09-13.
//

#ifndef RENDERER_MAIN_H
#define RENDERER_MAIN_H

#include "Vector3f.h"
#include "Ray.h"
#include "Hitable.h"
#include "Camera.h"
#include "Triangle.h"
#include "Light.h"

Vector3f color(const Ray& r, Hitable *world, int depth);
bool refract(const Vector3f& v, const Vector3f& n, float niOverNt, Vector3f& refracted);
float schlick(float cosine, float refIdx);
Vector3f randomInUnitDisk();
Vector3f randomInUnitSphere();
std::string render(Camera* cam, std::vector<Triangle> *geoList, std::vector<Light> *lightList);
Vector3f reflect(const Vector3f& v, const Vector3f& n);

#endif //RENDERER_MAIN_H
