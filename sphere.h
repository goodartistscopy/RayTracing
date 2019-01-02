#pragma once

#include "hitable.h"

#include <iostream>

class Material;


class Sphere : public Hitable
{
public:
    Sphere() { }

    Sphere(const Vec3 &_center, float _radius, Material *_material)
    : center(_center), radius(_radius), material(_material) { }

    virtual bool hit(const Ray &ray, float tmin, float tmax, Hit &hit) const override;
    
    virtual bool bounding_box(AABB &box) const override;

    inline void get_uv(const Vec3 &p, float &u, float &v) const
    {
        Vec3 q = (p - center) / radius;
        float phi = atan2(q.z(), q.x());
        float theta = asin(q.y());
        u = 1.0 - (phi + M_PI) / (2.0 * M_PI);
        v = (theta + M_PI/2.0) / M_PI;
    }

    friend std::ostream& operator<<(std::ostream &os, Sphere &s)
    {
        os << s.center << ", r = " << s.radius;
        return os;
    }

private:
    Vec3 center;
    float radius;

    Material *material;
};


bool Sphere::hit(const Ray &ray, float tmin, float tmax, Hit &hit) const
{
    Vec3 oc = ray.origin() - center;
    float a = dot(ray.direction(), ray.direction());
    float b = dot(oc, ray.direction());
    float c = dot(oc, oc) - radius * radius;
    
    float dis = b * b - a * c;
    if (dis > 0.0) {
        float t = (-b - sqrt(dis)) / a;
        if (t > tmin && t < tmax) {
            hit.p = ray.point_at_parameter(t);
            hit.normal = (ray.point_at_parameter(t) - center) / radius;
            hit.t = t;
            hit.material = material;
            get_uv(hit.p, hit.u, hit.v);
            return true;
        }

        t = (-b + sqrt(dis)) / a;
        if (t > tmin && t < tmax) {
            hit.p = ray.point_at_parameter(t);
            hit.normal = (ray.point_at_parameter(t) - center) / radius;
            hit.t = t;
            hit.material = material;
            get_uv(hit.p, hit.u, hit.v);
            return true;
        }
    }   

    return false;
}

bool Sphere::bounding_box(AABB &box) const
{
    box = AABB(center - radius, center + radius);
    
    return true;
}


