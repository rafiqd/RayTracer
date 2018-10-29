//
// Created by Rafiq on 2018-09-13.
//

#ifndef RENDERER_MAIN_H
#define RENDERER_MAIN_H

#include <thread>

#include "RNG.h"
#include "Vector3f.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Camera.h"
#include "Triangle.h"
#include "Light.h"


void threaded_render_pixel(int idx);

std::string render_scene();

extern Vector3f *output_img;
extern float *depths;
extern Camera *render_cam;
extern Hitable *world;
extern HitableList *hlist;
extern int *workedon;
extern int nx;
extern int ny;
extern int ns;
#endif //RENDERER_MAIN_H
