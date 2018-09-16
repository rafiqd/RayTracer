//
// Created by Rafiq on 2018-09-12.
//

#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "perlin.h"
#include "Vector3f.h"

class Texture  {
public:
    virtual Vector3f value(float u, float v, const Vector3f& p) const = 0;
};

class ConstantTexture : public Texture {
public:
    ConstantTexture() = default;
    explicit ConstantTexture(Vector3f c) : color(c) { }
    Vector3f value(float u, float v, const Vector3f& p) const override {
        return color;
    }
    Vector3f color;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture() = default;
    CheckerTexture(Texture *t0, Texture *t1): even(t0), odd(t1) { }
    Vector3f value(float u, float v, const Vector3f& p) const override {
        float sines = sinf(10*p.x())*sinf(10*p.y())*sinf(10*p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
    Texture *odd;
    Texture *even;
};


class NoiseTexture : public Texture {
public:
    NoiseTexture() {}
    explicit NoiseTexture(float sc) : scale(sc) {}
    Vector3f value(float u, float v, const Vector3f& p) const override {
        //return Vector3f(1,1,1)*0.5*(1 + noise.turb(scale * p));
        //return Vector3f(1,1,1)*noise.turb(scale * p);
        return Vector3f(1,1,1)*0.5*(1 + sin(scale*p.x() + 5*noise.turb(scale*p))) ;
    }
    perlin noise;
    float scale;
};

#endif //RENDERER_TEXTURE_H
