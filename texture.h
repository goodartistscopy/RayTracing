#pragma once

#include "vec3.h"
#include "perlin.h"
#include "utils.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual Vec3 value(float u, float v, const Vec3 &p) const = 0;
};

class ConstantTexture : public Texture
{
public:
    ConstantTexture() { }
    
    ConstantTexture(Vec3 c) : color(c) { }

    virtual Vec3 value(float u, float v, const Vec3 &p) const override
    {
        return color;
    }

private:
    Vec3 color;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture() {}

    CheckerTexture(const Vec3 &color1, const Vec3 &color2, float _scale = 10.0)
    : CheckerTexture(new ConstantTexture(color1), new ConstantTexture(color2), _scale) { }
    
    CheckerTexture(Texture *_texture1, Texture *_texture2, float _scale = 10.0)
    : texture1(_texture1), texture2(_texture2), scale(_scale) { }

    virtual Vec3 value(float u, float v, const Vec3 &p) const override
    {
        float sines = sin(scale*p.x()) * sin(scale*p.y()) * sin(scale*p.z());
        return (sines > 0.0) ? texture1->value(u, v, p) : texture2->value(u, v, p);
    }

private:
    Texture *texture1;
    Texture *texture2;
    float scale;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture() { }
    
    NoiseTexture(float _scale) : scale(_scale) { }

    virtual Vec3 value(float u, float v, const Vec3 &p) const override
    {
        //return Vec3(1, 1, 1) * 0.5 * (Perlin::noise(p * scale) + 1.0);
        //return Vec3(1, 1, 1) * Perlin::turb(p * scale);
        return Vec3(1, 1, 1) * 0.5 * (1.0 + sin(scale * p.z() + 10.0 * Perlin::turb(p)));
    }

private:
    float scale;
};


class ImageTexture : public Texture
{
public:
    ImageTexture() { }
    
    ImageTexture(uint8_t *_data, int _width, int _height, int _nc)
    : data(_data), width(_width), height(_height), nc(_nc)
    {
    
    }

    virtual Vec3 value(float u, float v, const Vec3 &p) const override
    {
        int i = clamp(int(u * width), 0, width);
        int j = clamp(int((1-v) * height), 0, height);
        
        float r = float(data[nc*(j * width + i) + 0]) / 255.0;
        float g = float(data[nc*(j * width + i) + 1]) / 255.0;
        float b = float(data[nc*(j * width + i) + 2]) / 255.0;
    
        return Vec3(r, g, b);
    }

private:
    uint8_t *data;
    int width;
    int height;
    int nc;
};



