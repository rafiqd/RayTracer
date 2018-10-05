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
    virtual float value(const Vector3f& direction) const { return 0; }
    virtual Vector3f generate() const { return Vector3f(0, 0, 0); }
};


class CosinePDF : public PDF {
public:
    ONB uvw;
    CosinePDF() = default;
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
    Hitable &ptr;
    HitablePDF(Hitable &p, const Vector3f& origin): ptr(p), o(origin) {}
    float value(const Vector3f& direction) const {
        return ptr.pdfValue(o, direction);
    }
    Vector3f generate() const {
        return ptr.random(o);
    }
};

class MixturePDF : public PDF {
public:
    PDF &_p1;
    PDF &_p2;

    MixturePDF(PDF &p0, PDF &p1): _p1(p0), _p2(p1) {}
    float value(const Vector3f& direction) const override {
        return 0.5f*_p1.value(direction) + 0.5f*_p2.value(direction);
    }

    Vector3f generate() const override {
        float r1 = gen.UniformFloat(0,1);
        if (r1 < 0.5){
            return _p1.generate();
        }
        return _p2.generate();
    }

};

#endif //RENDERER_PDF_H
