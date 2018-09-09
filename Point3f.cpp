//
// Created by Rafiq on 2018-08-26.
//

#include "Point3f.h"


std::ostream& operator<<(std::ostream &os, const Point3f p){
    return os << "P<" << p.x << ", " << p.y << ", " << p.z << ">";
}