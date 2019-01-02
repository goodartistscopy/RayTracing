#pragma once

#include "hitable.h"
#include "utils.h"
#include "texture.h"

class Material
{
public:
    virtual ~Material() = default;
    
    virtual bool scatter(const Ray &incoming, const Hit &hit, Vec3 &attenuation, Ray &scattered) const = 0;

    virtual Vec3 emitted(float u, float v, const Vec3 &p) const
    {
        return Vec3(0.0, 0.0, 0.0);
    }
};

class Lambertian : public Material
{
public:
    Lambertian(Texture *a) : albedo(a) { }

    virtual bool scatter(const Ray &incoming, const Hit &hit, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 target = hit.p + hit.normal + random_in_unit_sphere();
        scattered = Ray(hit.p, target - hit.p);
        attenuation = albedo->value(hit.u, hit.v, hit.p);
        return true;   
    }

private:
    Texture *albedo;
};

class Metal : public Material
{
public:
    Metal(Texture *a, float g = 0.0) : albedo(a), glossiness(g) { }

    virtual bool scatter(const Ray &incoming, const Hit &hit, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 reflected = reflect(unit_vector(incoming.direction()), hit.normal);
        scattered = Ray(hit.p, reflected + glossiness * random_in_unit_sphere());
        attenuation = albedo->value(hit.u, hit.v, hit.p);
        return (dot(scattered.direction(), hit.normal) > 0.0);
    }

private:
     
    Texture *albedo;
    float glossiness;
};


class Dielectric : public Material
{
public:
    Dielectric(float _ior) : ior(_ior) { };

    virtual bool scatter(const Ray &incoming, const Hit &hit, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 reflected = reflect(incoming.direction(), hit.normal);

        attenuation = Vec3(1.0, 1.0, 1.0);

        Vec3 outward_normal;
        float ni_over_nt;
        float cosine;
        if (dot(incoming.direction(), hit.normal) > 0) {
            outward_normal = -hit.normal;
            ni_over_nt = ior;
            // cos(theta_t)
            cosine = ior * dot(incoming.direction(), hit.normal) / incoming.direction().length();
        } else {
            outward_normal = hit.normal;
            ni_over_nt = 1.0 / ior;
            // cos(theta_i)
            cosine = -dot(incoming.direction(), hit.normal) / incoming.direction().length();
        }

        Vec3 refracted = 0;
        float reflect_prob;
        if (refract(incoming.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ior);
        } else {
            scattered = Ray(hit.p, reflected);
            reflect_prob = 1.0;   
        }
    
        if (random_in_0_1() < reflect_prob) {
            scattered = Ray(hit.p, reflected);
        } else { 
            scattered = Ray(hit.p, refracted);
        }

        return true;
    }

private:
    static float schlick(float cosine, float ior)
    {
        float r0 = (1 - ior) / (1 + ior);
        r0 *= r0;
        return r0 + (1 - r0) * pow(1 - cosine, 5);
    }

    float ior;
};


class DiffuseLight : public Material
{
public:
    DiffuseLight(Texture *e) : emit(e) { }

    virtual bool scatter(const Ray &incoming, const Hit &hit, Vec3 &attenuation, Ray &scattered) const override
    {
        return false;
    }

    virtual Vec3 emitted(float u, float v, const Vec3 &p) const override
    {
        return emit->value(u, v, p);
    }

private:
    Texture *emit;
};
