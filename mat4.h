#pragma once

#include "vec3.h"

class Vec4
{
public:
    Vec4() { }

    Vec4(float v) : Vec4(v,v,v,v) { }

    Vec4(float x, float y, float z, float w)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }

    Vec4(const Vec3 &p) {
        data[0] = p[0];
        data[1] = p[1];
        data[2] = p[2];
        data[3] = 1.0;
    }
    
    Vec4(const Vec3 &p, float w) {
        data[0] = p[0];
        data[1] = p[1];
        data[2] = p[2];
        data[3] = w;
    }

    inline float x() const { return data[0]; }

    inline float y() const { return data[1]; }

    inline float z() const { return data[2]; }

    inline float w() const { return data[3]; }

    inline float operator[](int i) const { return data[i]; }

    inline float& operator[](int i) { return data[i]; }

    inline Vec4& operator+=(const Vec4 &v)
    {
        data[0] += v.data[0];
        data[1] += v.data[1];
        data[2] += v.data[2];
        data[3] += v.data[3];

        return *this;
    }

    inline Vec3 to_vec3()
    {
        return Vec3(data[0], data[1], data[2]) / data[3];
    }

private:
    float data[4];
};


inline std::ostream& operator<<(std::ostream &os, const Vec4 &t)
{
    os << "[" << t[0] << ", " << t[1] << ", " << t[2] << ", " << t[3] << "]";
    return os;
}


class Mat4
{
public:
    Mat4() { }

    void setIdentity()
    {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                rows[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }
    }

    inline Vec4 & operator[](int i)
    {
        return rows[i];
    }

    inline Vec4 operator[](int i) const
    {
        return rows[i];
    }

private:
    Vec4 rows[4];
};

inline std::ostream& operator<<(std::ostream &os, const Mat4 &m)
{
    os << "[";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            os << m[i][j];
            if (j < 3) os << ", ";
        }
        if (i == 3) os << "]";
        else os << "\n";
    }
    return os;
}

inline Vec4 operator*(const Mat4 &mat, const Vec4 &vec)
{
    Vec4 res(0.0);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[i] += mat[i][j] * vec[j];
        }
    }

    return res;
}

inline Vec3 apply_transform_point(const Mat4 &M, const Vec3 &x)
{
    return (M * Vec4(x, 1.0)).to_vec3();
}

inline Vec3 apply_transform_vec(const Mat4 &M, const Vec3 &x)
{
    Vec4 x1 = M * Vec4(x, 0.0);
    return Vec3(x1.x(), x1.y(), x1.z());
}

inline Vec3 apply_inv_transform_point(const Mat4 &M, const Vec3 &x)
{
    Vec3 res(0.0);
    Vec3 x1 = x - Vec3(M[0][3], M[1][3], M[2][3]);
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            res[i] += M[j][i] * x1[j];
        }
    }

    return res;
}

inline Vec3 apply_inv_transform_vec(const Mat4 &M, const Vec3 &x)
{
    Vec3 res(0.0);
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            res[i] += M[j][i] * x[j];
        }
    }

    return res;
}

