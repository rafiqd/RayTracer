//
// Created by Rafiq on 2018-08-26.
//

#ifndef RENDERER_VECTOR_H
#define RENDERER_VECTOR_H

#include <iostream>
#include <cmath>

class Vector3f {
public:
    float v[3];

    Vector3f() = default;
    Vector3f(float u1, float u2, float u3){
            v[0] = u1;
            v[1] = u2;
            v[2] = u3;
    }
    inline float x() const { return v[0]; }
    inline float y() const { return v[1]; }
    inline float z() const { return v[2]; }

    inline const Vector3f& operator+() const { return *this; }
    inline Vector3f operator-() const { return Vector3f(-v[0], -v[1], -v[2]); }
    inline float operator[](int i) const { return v[i]; }
    inline float& operator[](int i) { return v[i]; }

    inline Vector3f& operator+=(const Vector3f &v1);
    inline Vector3f& operator-=(const Vector3f &v1);
    inline Vector3f& operator*=(const Vector3f &v1);
    inline Vector3f& operator/=(const Vector3f &v1);
    inline Vector3f& operator*=(const float t);
    inline Vector3f& operator/=(const float t);

    inline float length() const{
        return sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    }

    inline float squared_length() const{
        return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    }

    inline void make_unit_vector();

    friend inline std::ostream& operator<<(std::ostream &os, const Vector3f v){
        return os << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";
    }

    friend inline std::istream& operator>>(std::istream &is, Vector3f &t){
        is >> t.v[0] >> t.v[1] >> t.v[2];
        return is;
    }
};


inline void Vector3f::make_unit_vector() {
    float k = 1.0f / sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] *= k;
    v[1] *= k;
    v[2] *= k;
}

inline bool operator==(const Vector3f &v1, const Vector3f &v2){
    return v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z();
}

inline Vector3f operator+(const Vector3f &v1, const Vector3f &v2){
    return Vector3f(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2]);
}
inline Vector3f operator-(const Vector3f &v1, const Vector3f &v2){
    return Vector3f(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2]);
}
inline Vector3f operator*(const Vector3f &v1, const Vector3f &v2){
    return Vector3f(v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v1.v[2] * v2.v[2]);
}

inline Vector3f operator/(const Vector3f &v1, const Vector3f &v2){
    return Vector3f(v1.v[0] / v2.v[0], v1.v[1] / v2.v[1], v1.v[2] / v2.v[2]);
}

inline Vector3f operator*(float t, const Vector3f &v1){
    return Vector3f(t*v1.v[0], t*v1.v[1], t*v1.v[2]);
}

inline Vector3f operator*(const Vector3f &v1, float t){
    return Vector3f(t*v1.v[0], t*v1.v[1], t*v1.v[2]);
}

inline Vector3f operator/(const Vector3f &v1, float t){
    return Vector3f(v1.v[0]/t, v1.v[1]/t, v1.v[2]/t);
}

inline float dot(const Vector3f &v1, const Vector3f &v2){
    return v1.v[0]*v2.v[0] + v1.v[1]*v2.v[1] + v1.v[2]*v2.v[2];
}

inline Vector3f cross(const Vector3f &v1, const Vector3f &v2){
    return Vector3f( (v1.v[1]*v2.v[2] - v1.v[2]*v2.v[1]),
                     (-(v1.v[0]*v2.v[2] - v1.v[2]*v2.v[0])),
                     (v1.v[0]*v2.v[1] - v1.v[1]*v2.v[0]));
}
inline Vector3f& Vector3f::operator+=(const Vector3f &v1){
    v[0] += v1.v[0];
    v[1] += v1.v[1];
    v[2] += v1.v[2];
    return *this;
}
inline Vector3f& Vector3f::operator-=(const Vector3f &v1){
    v[0] -= v1.v[0];
    v[1] -= v1.v[1];
    v[2] -= v1.v[2];
    return *this;
}
inline Vector3f& Vector3f::operator*=(const Vector3f &v1){
    v[0] *= v1.v[0];
    v[1] *= v1.v[1];
    v[2] *= v1.v[2];
    return *this;
}
inline Vector3f& Vector3f::operator/=(const Vector3f &v1){
    v[0] /= v1.v[0];
    v[1] /= v1.v[1];
    v[2] /= v1.v[2];
    return *this;
}

inline Vector3f& Vector3f::operator*=(const float t){
    v[0] *= t;
    v[1] *= t;
    v[2] *= t;
    return *this;
}
inline Vector3f& Vector3f::operator/=(const float t){
    v[0] /= t;
    v[1] /= t;
    v[2] /= t;
    return *this;
}

inline Vector3f unit_vector(Vector3f v){
    return v / v.length();
}

#endif //RENDERER_VECTOR_H
