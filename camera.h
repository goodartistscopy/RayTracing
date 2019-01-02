#pragma once

#include "vec3.h"
#include "utils.h"

class Camera
{
public:
    Camera() { }

    Camera(float hfov, float aspect, float aperture = 1e-2, float focus_dist = 5.0f)
    {
        setup(hfov, aspect);

        look_at(Vec3(0.0), Vec3(0, 0, -1), Vec3(0, 1, 0));
        set_lens(aperture, focus_dist);
    }    

    void setup(float hfov, float aspect)
    {
        float theta = hfov * (M_PI/ 180.0);
        half_width = fabs(tan(theta/2.0));
        half_height = half_width / aspect;
    
        update_internals();
    }

    void look_at(const Vec3 &from, const Vec3 &to, const Vec3 &up)
    {
        origin = from;
        cz = unit_vector(from - to);
        cx = unit_vector(cross(up, cz));
        cy = cross(cz, cx);

        update_internals();
    }

    void set_lens(float aperture, float _focus_dist)
    {
        lens_radius = aperture / 2.0;
        focus_dist = _focus_dist;

        update_internals();
    }

    Ray get_ray(float u, float v)
    {
        Vec3 rd = lens_radius * random_in_unit_disk(); 
        Vec3 offset = rd.x() * cx + rd.y() * cy;
        return Ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - (origin + offset));
    }

private:
    void update_internals()
    {
        lower_left_corner = origin - focus_dist * (half_width * cx + half_height * cy + cz);
        horizontal = 2.0 * half_width * focus_dist * cx; 
        vertical = 2.0 * half_height * focus_dist * cy;        
    }

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
