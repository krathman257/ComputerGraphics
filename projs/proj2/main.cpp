#include <iostream>
#include <time.h>

#include <glm/glm.hpp>

#include <bitmap/bitmap_image.hpp>

#include "boundingbox.h"
#include "camera.h"
#include "hit.h"
#include "intersector.h"
#include "light.h"
#include "ray.h"
#include "renderer.h"
#include "shape.h"
#include "texture.h"
#include "timer.h"

//std::string projectPath = "C:\\Users\\Krathman257\\Desktop\\Homework\\Computer Graphics\\projs\\proj2\\";
std::string projectPath = "";

class MyIntersector : public Intersector {
public:
    Hit find_first_intersection(const std::vector<BoundingBox>& bb, const Ray& ray, const int& ttl) {
        Hit hit(ray);
        for (auto box : bb) {
            if (box.isHit(ray)) {
                for (auto surface : box.boxWorld.shapes()) {
                    double cur_t = surface->intersect(ray);
                    if (cur_t > 0.1 && cur_t < hit.t()) {
                        hit.update(surface, cur_t);
                    }
                }
            }
        }
        if (hit.is_intersection() && hit.reflect() && ttl > 0) {
            Ray ray_t;
            glm::vec3 pos = hit.position();
            glm::vec3 dir = ray.direction - 2 * (dot(hit.normal(), ray.direction)) * hit.normal();
            ray_t.origin = pos;
            ray_t.direction = dir;

            Hit hit_t = find_first_intersection(bb, ray_t, ttl-1);
            hit_t.set_is_reflection(true);
            return hit_t;
        }
        return hit;
    }
};

double rand_val() {
    static bool init = true;
    if (init) {
        srand(time(NULL));
        init = false;
    }
    return (double)rand() / (RAND_MAX);
}

glm::vec3 rand_color() {
    return glm::vec3(rand_val(),rand_val(),rand_val());
}

glm::vec3 normalize_color(float r, float g, float b) {
    return glm::vec3(r / 255, g / 255, b / 255);
}

int main(int argc, char** argv) {

    int inp_x, inp_y, inp_bool;
    bool inp_reflect, inp_flat;
    std::string m_shipPath;

    std::cout << "Enter a resolution to be rendered at (Suggest 320 240, 800 600, 2048 1536):" << std::endl;
    std::cin >> inp_x >> inp_y;

    std::cout << "Simple or detailed model? (0=Simple, 1=Detailed)" << std::endl;
    std::cin >> inp_bool;
    if(inp_bool == 1) { m_shipPath = projectPath + "models\\spaceship.obj"; }
    else { m_shipPath = projectPath + "models\\spaceship_simple.obj"; }

    std::cout << "Normal smoothing? (0=No, 1=Yes)" << std::endl;
    std::cin >> inp_bool;
    if (inp_bool == 1) { inp_flat = false; }
    else { inp_flat = true; }

    std::cout << "Reflections? (0=No, 1=Yes)" << std::endl;
    std::cin >> inp_bool;
    if (inp_bool == 1) { inp_reflect = true; }
    else { inp_reflect = false; }

    bitmap_image image(inp_x, inp_y);

    /*
    //bitmap_image image(320, 240);
    bitmap_image image(640, 480);
    //bitmap_image image(800, 600);
    //bitmap_image image(1280, 960);
    //bitmap_image image(2048, 1536);
    */

    float dist_to_origin = 5;
    Camera camera(
        glm::vec3(0, 0, -dist_to_origin),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec2(-5, -5),
        glm::vec2(5, 5),
        dist_to_origin,
        glm::vec3(0.15, 0.15, 0.15),
        glm::vec3(1, 1, 1)
    );

    PointLight l1(glm::vec3(1, 1, 1), glm::vec3(13, 0, -5), 1.0, 0.7, 0.18);
    DirectionalLight l2(glm::vec3(.5, .5, .5), glm::vec3(-5, 4, -1));
    Lights lights = { &l1, &l2 };

    std::vector<BoundingBox> boxes;

    //Jupiter texture
    glm::vec3 j_c_11 = normalize_color(242, 215, 170);
    glm::vec3 j_c_12 = normalize_color(155, 118, 74);
    glm::vec3 j_c_21 = normalize_color(229, 178, 147);
    glm::vec3 j_c_22 = normalize_color(156, 98, 74);

    TwoColorTexture j_t_1(25, 75, j_c_11, j_c_12);
    TwoColorTexture j_t_2(25, 75, j_c_21, j_c_22);

    MixedTexture jupiter(50, 125, 0.45, j_t_1, j_t_2);

    //Earth texture
    glm::vec3 e_c_11 = normalize_color(133, 165, 255);
    glm::vec3 e_c_12 = normalize_color(66, 117, 255);
    glm::vec3 e_c_21 = normalize_color(64, 128, 67);
    glm::vec3 e_c_22 = normalize_color(179, 134, 107);

    TwoColorTexture e_t_1(100, e_c_11, e_c_12);
    TwoColorTexture e_t_2(80, e_c_21, e_c_22);

    MixedTexture earth(60, 0.5, e_t_1, e_t_2);

    //Moon texture
    glm::vec3 m_c = normalize_color(200, 200, 200);

    OneColorTexture moon(350, m_c);

    //Create objects
    boxes.push_back(BoundingBox(Sphere(glm::vec3(-5, 1, 0), 4, jupiter)));
    boxes.push_back(BoundingBox(Sphere(glm::vec3(2, -1, -3), 1, earth)));
    boxes.push_back(BoundingBox(Sphere(glm::vec3(0.75, 0.5, -4), 0.15, moon)));

    boxes.push_back(BoundingBox(ImportedObj::import_obj(
        m_shipPath,
        glm::vec3(0, 0, 0),
        glm::vec3(0, 105, -15),
        glm::vec3(0.8, 0.8, 0.8),
        0.5,
        inp_flat,
        inp_reflect
    )));

    boxes.push_back(BoundingBox(ImportedObj::import_obj(
        m_shipPath,
        glm::vec3(2, 4, 4),
        glm::vec3(0, 85, 15),
        glm::vec3(0.8, 0.8, 0.8),
        0.5,
        inp_flat,
        inp_reflect
    )));

    for (int i = 0; i < boxes.size(); i++) {
        boxes[i].boxWorld.lock();
    }

    MyIntersector intersector1;
    Renderer renderer1(&intersector1);
    Timer timer;
    
    std::cout << "Rendering image..." << std::endl;
    timer.start();
    renderer1.render(image, camera, lights, boxes);
    timer.stop();
    image.save_image("RenderedImage.bmp");
    std::cout << "Image rendered in " << timer.total() / 60000 << " minutes" << std::endl;
    
}
