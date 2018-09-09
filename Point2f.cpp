//
// Created by Rafiq on 2018-08-26.
//

#include "Point2f.h"


std::ostream& operator<<(std::ostream &os, const Point2f p){
    return os << "P<" << p.x << ", " << p.y << ">";
}