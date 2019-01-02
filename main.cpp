#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "sphere.h"
#include "xy_rectangle.h"
#include "camera.h"
#include "material.h"
#include "utils.h"
#include "bvh_node.h"
#include "texture.h"
#include "object_frame.h"

#include <limits>
#include <iomanip>
#include <sstream>

Vec3 background(const Ray &r)
{
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return mix(Vec3(0.5, 0.7, 1.0), Vec3(1.0, 1.0, 1.0), t);
}

inline Vec3 trace_ray(const Ray &r, const Hitable &world, int depth = 0)
{
    Hit hit;
    if (world.hit(r, 0.001, std::numeric_limits<float>::max(), hit)) {
        Ray scattered;
        Vec3 attenuation;
        Vec3 emitted = hit.material->emitted(hit.u, hit.v, hit.p);
        if (depth < 10 && hit.material->scatter(r, hit, attenuation, scattered)) {
            return emitted + attenuation * trace_ray(scattered, world, depth + 1);
        } else {
            return emitted;
        }
    } else {
        return Vec3(0.0, 0.0, 0.0);
    }
}

void build_book_scene_bvh(HitableList &world, Camera &cam)
{
    cam.setup(20.0, 1.33333);
    cam.look_at(Vec3(-5, 2, 30), Vec3(0, 0, 0), Vec3(0, 1, 0));
    cam.set_lens(0.2, 30.0);

    world.add(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(new CheckerTexture(Vec3(0.5, 0.5, 0.6), Vec3(1.0, 1.0, 1.0)))));

    std::vector<Hitable *>elems;
        
    int n_spheres = 150;
    for (int k = 0; k < n_spheres; ++k) {
        float choose_mat = random_in_0_1();
        Material *mat;
        if (choose_mat > 0.3) {
            Vec3 color(random_in_0_1() * random_in_0_1(),
                       random_in_0_1() * random_in_0_1(),
                       random_in_0_1() * random_in_0_1());
            mat = new Lambertian(new ConstantTexture(color));
        } else if (choose_mat > 0.15) {
            Vec3 color(random_in_0_1(), random_in_0_1(), random_in_0_1());
            mat = new Metal(new ConstantTexture(color), random_in_0_1());
        } else {
            mat = new Dielectric(1.5);
        }
    
        Vec3 center(30*random_in_0_1() - 15.0, 0.5, 30*random_in_0_1() - 15.0);
        elems.push_back(new Sphere(center, 0.5, mat));
    }
    elems.push_back(new Sphere(Vec3(0, 2, 10), 2, new Metal(new ConstantTexture(Vec3(0.8, 0.8, 0.9)), 0.0)));
    elems.push_back(new Sphere(Vec3(0, 2, 0), 2, new Dielectric(1.5)));
    elems.push_back(new Sphere(Vec3(0, 2, -10), 2, new Lambertian(new ConstantTexture(Vec3(0.6, 0.3, 0.05)))));
    
    world.add(new BVHNode(elems));
}

void build_book_scene(HitableList &world, Camera &cam)
{
    cam.setup(20.0, 1.33333);
    cam.look_at(Vec3(-5, 2, 30), Vec3(0, 0, 0), Vec3(0, 1, 0));
    cam.set_lens(0.2, 30.0);

    world.add(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(new CheckerTexture(Vec3(0.5, 0.5, 0.6), Vec3(1.0, 1.0, 1.0)))));

    world.add(new Sphere(Vec3(0.0, 0.0, 0.0), 10000,
              new DiffuseLight(new ConstantTexture(Vec3(0.9, 0.9, 1.0)))));

    int n_spheres = 150;
    for (int k = 0; k < n_spheres; ++k) {
        float choose_mat = random_in_0_1();
        Material *mat;
        if (choose_mat > 0.3) {
            Vec3 color(random_in_0_1() * random_in_0_1(),
                       random_in_0_1() * random_in_0_1(),
                       random_in_0_1() * random_in_0_1());
            mat = new Lambertian(new ConstantTexture(color));
        } else if (choose_mat > 0.15) {
            Vec3 color(random_in_0_1(), random_in_0_1(), random_in_0_1());
            mat = new Metal(new ConstantTexture(color), random_in_0_1());
        } else {
            mat = new Dielectric(1.5);
        }
    
        Vec3 center(30*random_in_0_1() - 15.0, 0.5, 30*random_in_0_1() - 15.0);
        world.add(new Sphere(center, 0.5, mat));
    }
    world.add(new Sphere(Vec3(0, 2, 10), 2, new Metal(new ConstantTexture(Vec3(0.8, 0.8, 0.9)), 0.0)));
    world.add(new Sphere(Vec3(0, 2, 0), 2, new Dielectric(1.5)));
    world.add(new Sphere(Vec3(0, 2, -10), 2, new Lambertian(new ConstantTexture(Vec3(0.6, 0.3, 0.05)))));
}

void build_big_bvh(HitableList &world, Camera &cam)
{
    cam.setup(20.0, 1.33333);
    cam.look_at(Vec3(-5, 2, 30), Vec3(0, 0, 0), Vec3(0, 1, 0));
    cam.set_lens(0.2, 30.0);

    world.add(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(new CheckerTexture(Vec3(0.1, 0.7, 0.3),Vec3(1.0, 1.0, 1.0)))));

    world.add(new Sphere(Vec3(0.0, 0.0, 0.0), 10000,
              new DiffuseLight(new ConstantTexture(Vec3(0.9, 0.9, 1.0)))));

    int n_spheres = 750;

    std::vector<Hitable *> spheres;
    for (int i = 0; i < n_spheres; ++i) {
        Vec3 center(random_in_0_1(), random_in_0_1(), random_in_0_1());
        float radius = 0.2;
        center = 4.0 * (center - 0.5);
        center[1] += 3;

        //Material *mat = new Lambertian(Vec3(0.95, 0.95, 0.8));
        Material *mat = new Metal(new ConstantTexture(Vec3(0.95, 0.95, 0.8)), 0.2);
        spheres.push_back(new Sphere(center, radius, mat));
    }

    world.add(new BVHNode(spheres));
}

void build_test_perlin(HitableList &world, Camera &cam)
{
    cam.setup(20.0, 1.33333);
    cam.look_at(Vec3(-5, 2, 30), Vec3(0, 0, 0), Vec3(0, 1, 0));
    cam.set_lens(0.2, 30.0);

    world.add(new Sphere(Vec3(0, -1000, 0), 1000,
              new Metal(new ConstantTexture(Vec3(0.8, 0.8, 0.9)), 0.05)));

    world.add(new Sphere(Vec3(0, 3, 0), 3.0,
              new Lambertian(new NoiseTexture(5.0))));
}

void build_test_texture(HitableList &world, Camera &cam)
{
    cam.setup(20.0, 1.33333);
    cam.look_at(Vec3(-5, 2, 30), Vec3(0, 0, 0), Vec3(0, 1, 0));
    cam.set_lens(0.2, 30.0);

    world.add(new Sphere(Vec3(0, -1000, 0), 1000,
              new Metal(new ConstantTexture(Vec3(0.8, 0.8, 0.9)), 0.05)));

    world.add(new Sphere(Vec3(0.0, 0.0, 0.0), 10000,
              new DiffuseLight(new ConstantTexture(Vec3(0.9, 0.9, 1.0)))));

    int width, height, nc;
    uint8_t *tex_data = stbi_load("earthmap.jpg", &width, &height, &nc, 0);
    
    if (tex_data) {
        world.add(new Sphere(Vec3(0, 3.5, 0), 3.0,
                  new Lambertian(new ImageTexture(tex_data, width, height, nc))));
    }
}

void build_test_light(HitableList &world, Camera &cam)
{
    cam.setup(35.0, 1.33333);
    cam.look_at(Vec3(-5, 2, 40), Vec3(0, 0, 0), Vec3(0, 1, 0));
    cam.set_lens(0.2, 40.0);

    std::vector<Hitable *> elems;
    elems.push_back(new Sphere(Vec3(0, -1000, 0), 1000,
                         new Metal(new ConstantTexture(Vec3(0.8, 0.8, 0.9)), 0.1)));
 
    //elems.push_back(new Sphere(Vec3(0.0, 0.0, 0.0), 10000,
    //                new DiffuseLight(new ConstantTexture(Vec3(0.1, 0.1, 0.15)))));

    int width, height, nc;
    uint8_t *tex_data = stbi_load("data/earthmap.jpg", &width, &height, &nc, 0);
    
    if (tex_data) {
        elems.push_back(new Sphere(Vec3(0, 3.5, 0), 3.0,
                             new Lambertian(new ImageTexture(tex_data, width, height, nc))));
    }

    elems.push_back(new Sphere(Vec3(3.0, 1.0, 3.0), 0.5,
                         new DiffuseLight(new ConstantTexture(Vec3(5.0, 0.5, 0.0)))));

    Hitable *light = new XYRectangle(5, 5, new DiffuseLight(new ConstantTexture(Vec3(8.0, 8.0, 8.0))));
    ObjectFrame *light_frame = new ObjectFrame(light);
    light_frame->set_transform(35.0, 0.0, 0.0, 0.0, 7.0, -7.0);
    elems.push_back(light_frame);
    
    world.add(new BVHNode(elems)); 
}

int main(int argc, char *argv[])
{
    HitableList world;
    Camera cam;

    // intialize the random seed
    random_in_0_1();

    //build_book_scene_bvh(world, cam);
    build_book_scene(world, cam);
    
    //build_big_bvh(world, cam);
    
    //build_test_perlin(world, cam);
    //build_test_texture(world, cam);

    //build_test_light(world, cam);

    int width = 160;
    int height = 120;
    
    uint8_t * pixels = new uint8_t[3*width*height];
    
    int ns = 100;
    
    int start = 20;
    int stop = 21;
    int tmax = 100;
    const char * filename = "out/test_";
    for (int t = start; t < stop; t++) {
        float tm = tmax > 1 ? float(t) / (tmax-1) : 0.5;
        tm = (sin(-M_PI/2 + tm*M_PI) + 1.0) / 2.0;

        /*cam.set_lens(0.5, 30.0 - (20 * tm - 10));    
        float x = -9 + tm * 6;
        float y = 8 - tm;
        cam.look_at(Vec3(x, y, 30), Vec3(0, 0, 0), Vec3(0, 1, 0));
        */
        float x = 50.0 * sin(2*tm*M_PI);
        float y = 8 + 10 * 4*(tm-0.5)*(tm-0.5);
        float z = 50.0 * cos(2*tm*M_PI);
        //cam.look_at(Vec3(x, y, z), Vec3(0, 3, 0), Vec3(0, 1, 0));
        cam.look_at(Vec3(x, y, z), Vec3(0, 1, 0), Vec3(0, 1, 0));
        
        std::cout << "Rendering frame " << t << std::endl;

        Sphere::reset_stats();
    
        #pragma omp parallel for
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                Vec3 color(0.0);
                for (int s = 0; s < ns; ++s) {
                    float u = float(i + random_in_0_1()) / width;
                    float v = float(height - j + random_in_0_1()) / height;
            
                    Ray r = cam.get_ray(u, v);       

                    color += trace_ray(r, world);
                }
                color /= float(ns);
                pixels[3*(width*j + i)] = color.r8();
                pixels[3*(width*j + i)+1] = color.g8();
                pixels[3*(width*j + i)+2] = color.b8();
            }
        }

        std::cout << Sphere::get_stats() << " hit tests\n";    

        std::stringstream ss;
        ss << filename << std::setw(3) << std::setfill('0') << t << ".png";
        std::string fname = ss.str(); 
        stbi_write_png(fname.c_str(), width, height, 3, pixels, 3 * width);

        ss.str(std::string());
    }

    delete [] pixels;
    return 0;
}
