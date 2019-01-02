#pragma once

#include <math.h>
#include <iostream>

float clamp(float value, float min, float max)
{
    return (value > min) ? ((value < max) ? value : max) : min;
}

class Vec3
{
public:
    Vec3() {}

    Vec3(float t) { e[0] = e[1] = e[2] = t; }    

    Vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    
    inline float x() const { return e[0]; }
    
    inline float y() const { return e[1]; }
    
    inline float z() const { return e[2]; }

    inline float r() const { return e[0]; }
    
    inline float g() const { return e[1]; }
    
    inline float b() const { return e[2]; }
 
    inline uint8_t r8() { return uint8_t(sqrt(clamp(e[0], 0.0, 1.0))*255.99); }
    
    inline uint8_t g8() { return uint8_t(sqrt(clamp(e[1], 0.0, 1.0))*255.99); }
    
    inline uint8_t b8() { return uint8_t(sqrt(clamp(e[2], 0.0, 1.0))*255.99); }

    inline const Vec3& operator+() { return *this; }
    
    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    
    inline float operator[](int i) const { return e[i]; }
    
    inline float& operator[](int i) { return e[i]; }

    inline Vec3& operator+=(const Vec3 &v2);
    
    inline Vec3& operator-=(const Vec3 &v2);
    
    inline Vec3& operator*=(const Vec3 &v2);
    
    inline Vec3& operator*=(const float t);
    
    inline Vec3& operator/=(const Vec3 &v2);
    
    inline Vec3& operator/=(const float t);

    inline float length() const
    {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }

    inline float squared_length() const
    {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
    
    inline void make_unit_vector();

private:
    float e[3];
};

inline Vec3 operator/(const Vec3 &v, float t);

inline float dot(const Vec3 &v1, const Vec3 &v2);

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2);

inline Vec3 unit_vector(const Vec3 &v);

inline std::istream& operator>>(std::istream &is, Vec3 &t)
{
    is >> t[0] >> t[1] >> t[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3 &t)
{
    os << "[" << t[0] << ", " << t[1] << ", " << t[2] << "]";
    return os;
}
    
inline void Vec3::make_unit_vector()
{
    float k = 1.0 / length();
    *this *= k;
}

inline Vec3 unit_vector(const Vec3 &v)
{
    return v / v.length();
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

inline Vec3 operator*(float t, const Vec3 &v)
{
    return Vec3(t * v[0], t * v[1], t * v[2]);
}

inline Vec3 operator*(const Vec3 &v, float t)
{
    return Vec3(t * v[0], t * v[1], t * v[2]);
}

inline Vec3 operator/(const Vec3 &v, float t)
{
    return Vec3(v[0] / t, v[1] / t, v[2] / t);
}

inline float dot(const Vec3 &v1, const Vec3 &v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3( (v1[1] * v2[2]) - (v1[2] * v2[1]),
                 (v1[2] * v2[0]) - (v1[0] * v2[2]),
                 (v1[0] * v2[1]) - (v1[1] * v2[0]) );
}

inline Vec3& Vec3::operator+=(const Vec3 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3 &v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];

    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];

    return *this;
}

inline Vec3& Vec3::operator*=(float t)
{
    (*this)[0] *= t;
    (*this)[1] *= t;
    (*this)[2] *= t;

    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
}

inline Vec3& Vec3::operator/=(float t)
{
    (*this)[0] /= t;
    (*this)[1] /= t;
    (*this)[2] /= t;

    return *this;
}

inline Vec3 mix(const Vec3 &v1, const Vec3 &v2, float t)
{
    return t * v1 + (1.0f - t) * v2;
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
    return v - 2.0f * dot(v, n) * n;
}

inline bool refract(const Vec3 &v, const Vec3 &n, float ni_over_nt, Vec3 &r)
{
    Vec3 uv = unit_vector(v);
    float d = dot(uv, n);
    float dis = 1.0f - (ni_over_nt * ni_over_nt) * (1 - d * d);
    if (dis >= 0) {
        r = ni_over_nt * uv - (ni_over_nt * d + sqrt(dis)) * n;
        return true;
    } else {
        return false;   
    }
}
