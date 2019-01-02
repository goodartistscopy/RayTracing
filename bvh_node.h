#pragma once

#include "hitable.h"
#include "sphere.h"

#include <algorithm>
#include <iostream>

class BVHNode : public Hitable
{
public:
    BVHNode() {}

    BVHNode(std::vector<Hitable *>hitables)
    : BVHNode(hitables.begin(), hitables.end(), 0) { }
    
    template<typename RandomIt>
    BVHNode(RandomIt begin, RandomIt end, int depth)
    {
        int n = std::distance(begin, end);
        
        int axis = int(3.0 * random_in_0_1());
        if (axis == 0) {
            std::sort(begin, end, hitable_compare<0>);
        } else if (axis == 1) {
            std::sort(begin, end, hitable_compare<1>);
        } else if (axis == 2) {
            std::sort(begin, end, hitable_compare<2>);
        }

        if (n == 1) {
            left = right = *begin;
        } else if (n == 2) {
            left = *begin;
            right = *(begin+1);
        } else {
            left = new BVHNode(begin, begin + n/2, depth + 1);   
            right = new BVHNode(begin + n/2, end, depth + 1);   
        }

        AABB left_box, right_box;
        if (!left->bounding_box(left_box) || !right->bounding_box(right_box)) {
            std::cerr << "Error: trying to include infinite object in a BVH" << std::endl;
        }
        box = surrounding_box(left_box, right_box);
    }

    virtual bool hit(const Ray &ray, float tmin, float tmax, Hit &hit) const override
    {
        if (box.hit(ray, tmin, tmax)) {
            Hit left_hit, right_hit;
            bool hit_left = left->hit(ray, tmin, tmax, left_hit);
            bool hit_right = right->hit(ray, tmin, tmax, right_hit);
            if (hit_left && hit_right) {
                if (left_hit.t < right_hit.t) {
                    hit = left_hit;
                } else {
                    hit = right_hit;
                }
                return true;
            } else if (hit_left) {
                hit = left_hit;
                return true;
            } else if (hit_right) {
                hit = right_hit;
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
   
    virtual bool bounding_box(AABB &_box) const override
    {
        _box = box;
        return true;
    }

private:
    Hitable *left;
    Hitable *right;
    AABB box;
};


