#pragma once

#include <math.h>
#include <limits>

#include "vec3.h"

static thread_local uint32_t s_RndState = 1;

inline uint32_t XorShift32()
{
    uint32_t x = s_RndState;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 15;
    s_RndState = x;
    return x;
}

inline float my_random()
{
    return float(XorShift32()) / std::numeric_limits<uint32_t>::max();
}

inline float random_in_0_1()
{
    return my_random();
}

inline Vec3 random_in_unit_sphere()
{
    Vec3 v;
    do {
        v = 2.0f * Vec3(random_in_0_1(), random_in_0_1(), random_in_0_1()) - 1.0f;
    } while (v.squared_length() > 1.0);

    return v;
}

inline Vec3 random_in_unit_disk()
{
    Vec3 v;
    do {
        v = 2.0f * Vec3(random_in_0_1(), random_in_0_1(), 0.0) - Vec3(1.0, 1.0, 0.0);
    } while (v.squared_length() > 1.0);

    return v;
}

inline float fast_min(float a, float b)
{
    return (a < b) ? a : b;
}

inline float fast_max(float a, float b)
{
    return (a > b) ? a : b;
}

template<typename T>
inline T clamp(T value, T min, T max)
{
    return (value > min) ? ((value < max) ? value : max) : min;
}
