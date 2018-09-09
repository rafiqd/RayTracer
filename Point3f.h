//
// Created by Rafiq on 2018-08-26.
//

#ifndef RENDERER_POINT3F_H
#define RENDERER_POINT3F_H

#include <iosfwd>
#include <iostream>

class Point3f {
public:
    float x;
    float y;
    float z;

    Point3f(float p1, float p2, float p3): x(p1), y(p2), z(p3){}
};

std::ostream& operator<<(std::ostream&, Point3f);

#endif //RENDERER_POINT3F_H
