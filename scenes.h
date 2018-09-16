//
// Created by Rafiq on 2018-09-16.
//

#ifndef RENDERER_SCENES_H
#define RENDERER_SCENES_H

#include "Hitable.h"
#include "Lambertian.h"
#include "Rectangle.h"
#include "ConstantMedium.h"
#include "Sphere.h"
#include "BVHNode.h"
#include "HitableList.h"
#include "Material.h"

Hitable* cornell_box();
Hitable* cornell_smoke();
Hitable *simple_light();
Hitable *two_perlin_spheres();
Hitable *two_spheres();
Hitable* randomScene();


#endif //RENDERER_SCENES_H
