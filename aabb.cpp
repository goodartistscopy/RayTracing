#include "aabb.h"
#include "utils.h"
#include "vec3.h"
#include "mat4.h"
#include "ray.h"

bool AABB::hit(const Ray &ray, float tmin, float tmax) const
{ 
    for (int a = 0; a < 3; a++) {
        float invd = 1.0 / ray.direction()[a];
        float t0 = (bmin[a] - ray.origin()[a]) * invd;
        float t1 = (bmax[a] - ray.origin()[a]) * invd;
        if (invd < 0.0) {
            std::swap(t0, t1);
        }

        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;

        if (tmin >= tmax) {
            return false;
        }
    }
    return true;
}

AABB surrounding_box(const AABB &box0, const AABB &box1)
{
    Vec3 small(fast_min(box0.min()[0], box1.min()[0]),
               fast_min(box0.min()[1], box1.min()[1]),
               fast_min(box0.min()[2], box1.min()[2]));

    Vec3 big(fast_max(box0.max()[0], box1.max()[0]),
             fast_max(box0.max()[1], box1.max()[1]),
             fast_max(box0.max()[2], box1.max()[2]));

    return AABB(small, big);
}

AABB transform_bounding_box(const AABB &box, const Mat4 &T)
{
    Vec3 corners[8] =
    {
        Vec3(box.min()[0], box.min()[1], box.min()[2]),
        Vec3(box.max()[0], box.min()[1], box.min()[2]),
        Vec3(box.max()[0], box.max()[1], box.min()[2]),
        Vec3(box.min()[0], box.max()[1], box.min()[2]),
        Vec3(box.min()[0], box.min()[1], box.max()[2]),
        Vec3(box.max()[0], box.min()[1], box.max()[2]),
        Vec3(box.max()[0], box.max()[1], box.max()[2]),
        Vec3(box.min()[0], box.max()[1], box.max()[2])
    };

    Vec3 tmin(std::numeric_limits<float>::max());
    Vec3 tmax(std::numeric_limits<float>::lowest());
    for (int i = 0; i < 8; ++i) {
        Vec3 tc = apply_transform_point(T, corners[i]);
        for (int a = 0; a < 3; a++) {
            if (tc[a] < tmin[a]) {
                tmin[a] = tc[a];
            } else if (tc[a] > tmax[a]) {
                tmax[a] = tc[a];
            }
        }
    }
    
    return AABB(tmin, tmax);
}
