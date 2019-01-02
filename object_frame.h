#pragma once

#include "hitable.h"
#include "mat4.h"

#include <math.h>
#include <iostream>

class ObjectFrame : public Hitable
{
public:
    ObjectFrame(Hitable *_hitable)
    {
        hitable = _hitable;
        transform.setIdentity();
    }

    void set_transform(float rx, float ry, float rz, float tx, float ty, float tz)
    {
        float cx = cos(rx*M_PI/180);
        float sx = sin(rx*M_PI/180);
        float cy = cos(ry*M_PI/180);
        float sy = sin(ry*M_PI/180);
        float cz = cos(rz*M_PI/180);
        float sz = sin(rz*M_PI/180);
    
        Mat4 &d = transform;

        d[0][0] = cy*cz;            d[0][1] = -cy*sz;           d[0][2] = sy;
        d[1][0] = cx*sz + cz*sx*sy; d[1][1] = cx*cz - sx*sy*sz; d[1][2] = -cy*sx;
        d[2][0] = sx*sz - cx*cz*sy; d[2][1] = cz*sx + cx*sy*sz; d[2][2] = cx*cy;

        d[0][3] = tx; // d[0][0] * tx + d[1][0] * ty + d[2][0] * tz;
        d[1][3] = ty; //d[0][1] * tx + d[1][1] * ty + d[2][1] * tz;
        d[2][3] = tz; // d[0][2] * tx + d[1][2] * ty + d[2][2] * tz;
        d[3][0] = 0.0;
        d[3][1] = 0.0;
        d[3][2] = 0.0;
        d[3][3] = 1.0;
    }

    virtual bool hit(const Ray &ray, float tmin, float tmax, Hit &hit) const override
    {
        Vec3 torig = apply_inv_transform_point(transform, ray.origin());
        Vec3 tdir = apply_inv_transform_vec(transform, ray.direction());
    
        if (hitable->hit(Ray(torig, tdir), tmin, tmax, hit)) {
            hit.p = apply_transform_point(transform, hit.p);
            hit.normal = apply_transform_vec(transform, hit.normal);
            return true;
        }

        return false;
    }
   
    virtual bool bounding_box(AABB &box) const override
    {
        if (hitable->bounding_box(box)) {
            box = transform_bounding_box(box, transform);            
            return true;
        } else {
            return false;
        }
    }

private:
    Hitable *hitable;
    Mat4 transform;
};

