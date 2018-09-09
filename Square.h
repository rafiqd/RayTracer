//
// Created by Rafiq on 2018-08-26.
//

#ifndef RENDERER_SQUARE_H
#define RENDERER_SQUARE_H

#include <vector>

#include "Point3f.h"
#include "Point2f.h"
#include "Vector3f.h"
#include "Geometry.h"

class Square : public Geometry {
public:
    Point3f *point4;
    Point2f *texture4;
    Vector3f *normal4;

    Square(Point3f *p1, Point3f *p2, Point3f *p3, Point3f *p4,
           Point2f *t1, Point2f *t2, Point2f *t3, Point2f *t4,
           Vector3f *n1, Vector3f *n2, Vector3f *n3, Vector3f *n4):
            Geometry(p1, p2, p3, t1, t2, t3, n1, n2, n3) {
        point4 = p4;
        texture4 = t4;
        normal4 = n4;
    }

    virtual std::ostream& print(std::ostream& out) const override {
        out << "square";
        return out;
    }
};


#endif //RENDERER_SQUARE_H
