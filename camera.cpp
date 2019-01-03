#include "camera.h"
#include "vec3.h"
#include "ray.h"
#include "utils.h"

Camera::Camera() { }

Camera::Camera(float hfov, float aspect, float aperture, float focus_dist)
{
    setup(hfov, aspect);

    look_at(Vec3(0.0), Vec3(0, 0, -1), Vec3(0, 1, 0));
    set_lens(aperture, focus_dist);
}    

void Camera::setup(float hfov, float aspect)
{
    float theta = hfov * (M_PI/ 180.0);
    half_width = fabs(tan(theta/2.0));
    half_height = half_width / aspect;

    update_internals();
}

void Camera::look_at(const Vec3 &from, const Vec3 &to, const Vec3 &up)
{
    origin = from;
    cz = unit_vector(from - to);
    cx = unit_vector(cross(up, cz));
    cy = cross(cz, cx);

    update_internals();
}

void Camera::set_lens(float aperture, float _focus_dist)
{
    lens_radius = aperture / 2.0;
    focus_dist = _focus_dist;

    update_internals();
}

Ray Camera::get_ray(float u, float v) const
{
    Vec3 rd = lens_radius * random_in_unit_disk(); 
    Vec3 offset = rd.x() * cx + rd.y() * cy;
    return Ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - (origin + offset));
}

Vec3 Camera::project(Vec3 x) const
{
    Vec3 ox = x - origin;
    Vec3 xc(dot(cx, ox), dot(cy, ox), dot(cz, ox));
    Vec3 xp = xc / -xc.z();
    return Vec3(xp.x(), xp.y(), xp.z()) + 0.5;
}

void Camera::update_internals()
{
    lower_left_corner = origin - focus_dist * (half_width * cx + half_height * cy + cz);
    horizontal = 2.0 * half_width * focus_dist * cx; 
    vertical = 2.0 * half_height * focus_dist * cy;        
}


