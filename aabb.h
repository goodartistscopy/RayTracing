#pragma once

#include "vec3.h"
//#include "ray.h"

#include <utility>
#include <limits>

class Mat4;
class Ray;

class AABB
{
public:
    AABB() { }

    AABB(const Vec3 &_min, const Vec3 &_max) : bmin(_min), bmax(_max) { }

    Vec3 min() const { return bmin; };
    Vec3 max() const { return bmax; };

    bool hit(const Ray &ray, float tmin, float tmax) const;

private:
    Vec3 bmin;
    Vec3 bmax;
};

AABB surrounding_box(const AABB &box0, const AABB &box1);

AABB transform_bounding_box(const AABB &box, const Mat4 &T);

