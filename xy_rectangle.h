#pragma once

#include "vec3.h"
#include "hitable.h"
#include "material.h"

class XYRectangle : public Hitable
{
public:
    XYRectangle() { }
    
    XYRectangle(float width, float height, Material *mat) : x0(-width/2), y0(-height/2), x1(width/2), y1(height/2), material(mat) { }

    virtual bool hit(const Ray &ray, float tmin, float tmax, Hit &hit) const override
    {
        float ti = -ray.origin().z() / ray.direction().z();
        if ((ti < tmin) || (ti > tmax)) {
            return false;
        }

        float xi = ray.origin().x() + ray.direction().x() * ti; 
        float yi = ray.origin().y() + ray.direction().y() * ti;
        if ((xi > x0) && (xi < x1) && (yi > y0) & (yi < y1)) {
            hit.p = Vec3(xi, yi, 0.0);
            hit.normal = Vec3(0.0, 0.0, 1.0);
            hit.u = (xi - x0) / (x1 - x0);
            hit.v = (yi - y0) / (y1 - y0);
            hit.t = ti;
            hit.material = material;
            return true;
        } else {
            return false;
        } 
    }
   
    virtual bool bounding_box(AABB &box) const override
    {
        box = AABB(Vec3(x0, y0, 0.0), Vec3(x1, y1, 0.0));
        return true;
    }   

private:
    float x0;
    float y0;
    float x1;
    float y1;

    Material *material;
};
