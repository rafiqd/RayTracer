//
// Created by Rafiq on 2018-09-16.
//

#ifndef RENDERER_PDF_H
#define RENDERER_PDF_H

#include "Vector3f.h"
#include "ONB.h"
#include "main.h"

class PDF {
public:
    virtual float value(const Vector3f& direction) const = 0;
    virtual Vector3f generate() const = 0;
};

class CosinePDF : public PDF {
public:
    ONB uvw;

    explicit CosinePDF(const Vector3f& w) { uvw.buildFromW(w); }
    float value(const Vector3f &direction) const override  {
        float cosine = dot(unit_vector(direction), uvw.w());
        if (cosine > 0)
            return cosine/float(M_PI);
        return 0;
    }

    Vector3f generate() const override {
        return uvw.local(randomCosineDireciton());
    }
};

class HitablePDF : public PDF {
public:
    Vector3f o;
    Hitable *ptr;
    HitablePDF(Hitable *p, const Vector3f& origin): ptr(p), o(origin) {}
    float value(const Vector3f& direction) const {
        return ptr->pdfValue(o, direction);
    }
    Vector3f generate() const {
        return ptr->random(o);
    }
};

class MixturePDF : public PDF {
public:
    PDF *p[2];

    MixturePDF(PDF *p0, PDF *p1) { p[0] = p0; p[1] = p1; }
    float value(const Vector3f& direction) const override {
        return 0.5f*p[0]->value(direction) + 0.5f*p[1]->value(direction);
    }

    Vector3f generate() const override {
        if (drand48() < 0.5){
            return p[0]->generate();
        }
        return p[1]->generate();
    }

};

#endif //RENDERER_PDF_H
