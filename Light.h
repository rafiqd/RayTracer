//
// Created by Rafiq on 2018-08-30.
//

#ifndef RENDERER_LIGHT_H
#define RENDERER_LIGHT_H

#include <string>

class Light {
public:
    std::string name;
    float intensity;
    float color[3];
    float translate[3];
    float rotate[3];
    float scale[3];

    Light(std::string lightName, float lightIntensity,
          float r, float g, float b,
          float tx, float ty, float tz,
          float rx, float ry, float rz,
          float sx, float sy, float sz): intensity(lightIntensity),
                                         color{r,g,b},
                                         translate{tx, ty, tz},
                                         rotate{rx, ry, rz},
                                         scale{sx, sy, sz}{
        name = std::move(lightName);
    }
};


#endif //RENDERER_LIGHT_H
