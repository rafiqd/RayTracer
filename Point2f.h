//
// Created by Rafiq on 2018-08-26.
//

#ifndef RENDERER_POINT2F_H
#define RENDERER_POINT2F_H

#include <iosfwd>
#include <iostream>
#include "Vector3f.h"

class Point2f {
public:
    float x;
    float y;

    Point2f(float p1, float p2): x(p1), y(p2) {}
};

std::ostream& operator<<(std::ostream&, Point2f);



#endif //RENDERER_POINT2F_H
