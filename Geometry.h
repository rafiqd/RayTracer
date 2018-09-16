//
// Created by Rafiq on 2018-08-28.
//

#ifndef RENDERER_GEOMETRY_H
#define RENDERER_GEOMETRY_H


#include "Point3f.h"
#include "Vector3f.h"

class Geometry {
public:

    Point3f *point1;
    Point3f *point2;
    Point3f *point3;
    Vector3f *texture1;
    Vector3f *texture2;
    Vector3f *texture3;
    Vector3f *normal1;
    Vector3f *normal2;
    Vector3f *normal3;


    Geometry(Point3f *p1, Point3f *p2, Point3f *p3,
             Vector3f *t1, Vector3f *t2, Vector3f *t3,
             Vector3f *n1, Vector3f *n2, Vector3f *n3){
        point1 = p1;
        point2 = p2;
        point3 = p3;

        texture1 = t1;
        texture2 = t2;
        texture3 = t3;

        normal1 = n1;
        normal2 = n2;
        normal3 = n3;
    }

    friend std::ostream& operator<<(std::ostream &out, const Geometry &g){
        return g.print(out);
    }

    virtual std::ostream& print(std::ostream& out) const {
        out << "Geo";
        return out;
    }

};


#endif //RENDERER_GEOMETRY_H
