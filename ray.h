#pragma once

#include "vec3.h"

class Ray
{
public:
    Ray() {}

    Ray(const Vec3 &_O, const Vec3 &_d) : O(_O), d(_d) { }

    Vec3 origin() const { return O; }

    Vec3 direction() const { return d; }

    Vec3 point_at_parameter(float t) const { return O + t * d; }

private:
    Vec3 O;
    Vec3 d;
};
