#pragma once

#include "vec3.h"

class Ray;

class Camera
{
public:
    Camera();

    Camera(float hfov, float aspect, float aperture = 1e-2, float focus_dist = 5.0f);
  
    void setup(float hfov, float aspect);
  
    void look_at(const Vec3 &from, const Vec3 &to, const Vec3 &up);
  
    void set_lens(float aperture, float _focus_dist);
  
    Ray get_ray(float u, float v) const;
  
private:
    void update_internals();
 
    float half_width;
    float half_height;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 origin;
    float lens_radius;
    float focus_dist;
    Vec3 cz;
    Vec3 cx;
    Vec3 cy;
};
