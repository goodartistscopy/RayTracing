#pragma once

#include "vec3.h"
#include "aabb.h"

#include <vector>

class Ray;
class Material;

struct Hit
{   
    Hit() = default;
    //Hit(const Hit &h): p(h.p), normal(h.normal), t(h.t), material(h.material) { }

    Vec3 p;
    Vec3 normal;
    float u, v;
    float t;
    Material *material;
};

class Hitable
{
public:
    virtual bool hit(const Ray &ray, float tmin, float tmax, Hit &hit) const = 0;
   
    // return false if the object is not bounded
    virtual bool bounding_box(AABB &box) const = 0;
};

class HitableList : public Hitable
{
public:
    HitableList() { }
    
    void add(Hitable *hitable)
    {
        elems.push_back(hitable);
    }

    virtual bool hit(const Ray &ray, float tmin, float tmax, Hit &hit) const override
    {
        float t = tmax;
        bool got_hit = false;
        for(Hitable *e : elems) {
            Hit elem_hit;
            if (e->hit(ray, tmin, tmax, elem_hit)) {
                if (elem_hit.t < t) {
                    hit = elem_hit;
                    t = elem_hit.t;
                }
                got_hit = true;
            }
        }

        return got_hit;
    }

    virtual bool bounding_box(AABB &box) const override
    {
        if (elems.empty()) return false;
        
        AABB temp_box; 
        if (!elems[0]->bounding_box(temp_box)) {
            return false;
        } else {
            box = temp_box;
        }

        for (size_t i = 0; i < elems.size(); ++i) {
            Hitable *elem = elems[i];    
            AABB elem_box;
            if (elem->bounding_box(elem_box)) {
                box = surrounding_box(box, elem_box);
            } else {
                return false;
            }
        }
         
        return true;
    }

private:
    std::vector<Hitable *> elems;
};

template<int axis>
bool hitable_compare(Hitable * const &a, Hitable * const &b)
{
    AABB a_box, b_box;
    if (a->bounding_box(a_box) && b->bounding_box(b_box)) {
        return (a_box.min()[axis] < b_box.min()[axis]);
    } else {
        std::cerr << "Error: can't compare with an infinite object\n";
        return false;
    }
}

