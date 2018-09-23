//
// Created by Rafiq on 2018-08-30.
//

#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include <ostream>

#include "Ray.h"

Vector3f randomInUnitDisk();

class Camera {
public:

    Vector3f translate;
    Vector3f rotate;
    Vector3f scale;
    float focalLength;
    float lensRaidus;
    float time0;
    float time1;

    Vector3f u;
    Vector3f v;
    Vector3f w;
    Vector3f origin;
    Vector3f lowerLeftCorner;
    Vector3f horizontal;
    Vector3f vertical;

//    Camera(float vfov, float aspect){
//
//        float theta = vfov*float(M_PI)/180;
//        float half_height = tanf(theta/2);
//        float half_width = aspect * half_height;
//
//        lowerLeftCorner = Vector3f(-half_width, -half_height, -1.0f);
//        horizontal = Vector3f(2*half_width, 0.0, 0.0);
//        vertical = Vector3f(0.0, 2*half_height, 0.0);
//        origin = Vector3f(0,0,0);
//    }
//
//    Camera(Vector3f t, Vector3f r, Vector3f s,
//           float focal, float vfov, float aspect):
//           translate(t), rotate(t), scale(s), focalLength(focal) {
//
//        Vector3f u, v, w;
//        float theta = vfov*float(M_PI)/180;
//        float half_height = tanf(theta/2);
//        float half_width = aspect * half_height;
//
//        lowerLeftCorner = Vector3f(-half_width, -half_height, -1.0f);
//        horizontal = Vector3f(2*half_width, 0.0, 0.0);
//        vertical = Vector3f(0.0, 2*half_height, 0.0);
//    }

    Camera(Vector3f lookfrom, Vector3f lookat, Vector3f vup,
           float vfov, float aspect, float aperture, float focusDist,
           float t0, float t1){
        time0 = t0;
        time1 = t1;
        lensRaidus = aperture / 2;
        float theta = vfov*float(M_PI)/180;
        float half_height = tanf(theta/2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lowerLeftCorner = origin - half_width*focusDist*u -half_height*focusDist*v - focusDist*w;
        horizontal = 2*half_width*focusDist*u;
        vertical = 2*half_height*focusDist*v;
    }

    friend std::ostream& operator<<(std::ostream &out, const Camera &t){
        out << "Camera([" << t.translate[0] << ", " << t.translate[1] << ", " << t.translate[2] << "]\n["
                << t.rotate[0] << ", " << t.rotate[1] << ", " << t.rotate[2] << "]\n["
                << t.scale[0] << ", " << t.scale[1] << ", " << t.scale[2] << "]," << t.focalLength << ")" ;
        return out;
    }

    Ray getRay(float s, float t){
        Vector3f rd = lensRaidus * randomInUnitDisk();
        Vector3f offset = u * rd.x() + v * rd.y();
        float time = time0 + drand48() * (time1 - time0);

        Vector3f org = origin + offset;
        Vector3f dir = lowerLeftCorner + s*horizontal + t*vertical - origin - offset;


        return Ray(org, dir, time);
    }
};


#endif //RENDERER_CAMERA_H
