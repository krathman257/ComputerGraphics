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
#include "timer.h"


class BruteForceIntersector : public Intersector {
public:
    Hit find_first_intersection(const World& world, const Ray& ray) {
        Hit hit(ray);
        for (auto surface : world.shapes()) {
            double cur_t = surface->intersect(ray);
            if (cur_t < hit.t()) {
                hit.update(surface, cur_t);
            }
        }
        return hit;
    }

    Hit find_first_intersection(const std::vector<BoundingBox>& bb, const Ray& ray) { return Hit(ray); }
};

class MySlickIntersector : public Intersector {
public:
    Hit find_first_intersection(const std::vector<BoundingBox>& bb, const Ray& ray) {
        Hit hit(ray);
        for (auto box : bb) {
            if (box.isHit(ray)) {
                for (auto surface : box.boxWorld.shapes()) {
                    double cur_t = surface->intersect(ray);
                    if (cur_t < hit.t()) {
                        hit.update(surface, cur_t);
                    }
                }
            }
        }
        return hit;
    }

    Hit find_first_intersection(const World& world, const Ray& ray) { return Hit(ray); }
};

class MySlickIntersector2 : public Intersector {
public:
    Hit find_first_intersection(const std::vector<BoundingBox>& bb, const Ray& ray) {
        Hit hit(ray);
        for (auto box : bb) {
            if (box.isHit(ray)) {
                find_first_intersection(box.childBoxes, ray, hit);
                for (auto surface : box.boxWorld.shapes()) {
                    double cur_t = surface->intersect(ray);
                    if (cur_t < hit.t()) {
                        hit.update(surface, cur_t);
                    }
                }
            }
        }
        return hit;
    }

    void find_first_intersection(const std::vector<BoundingBox>& bb, const Ray& ray, Hit& hit) {
        for (auto box : bb) {
            if (box.isHit(ray)) {
                find_first_intersection(box.childBoxes, ray, hit);
                for (auto surface : box.boxWorld.shapes()) {
                    double cur_t = surface->intersect(ray);
                    if (cur_t < hit.t()) {
                        hit.update(surface, cur_t);
                    }
                }
            }
        }
    }

    Hit find_first_intersection(const World& world, const Ray& ray) { return Hit(ray); }
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

void addSphere(std::vector<BoundingBox>& bb, const Sphere& s) {
    BoundingBox temp = BoundingBox(s.center(), s.diameter());
    temp.addChildSphere(s);
    bb.push_back(temp);
}

std::vector<BoundingBox> processBoundingBoxes(std::vector<BoundingBox>& bb) {
    std::vector<BoundingBox> t1 = bb, t2;
    while (t1.size() > 1) {
        t2.clear();
        for (int i = 0; t1.size() > 1; i) {
            float bestDist = std::numeric_limits<float>::infinity();
            int index = -1;
            for (int j = 1; j < t1.size(); j++) {
                float foundDist = t1[0].distance(t1[j]);
                if (foundDist < bestDist) {
                    bestDist = foundDist;
                    index = j;
                }
            }
            t2.push_back(BoundingBox(t1[0], t1[index]));
            t1.erase(t1.begin());
            t1.erase(t1.begin() + index-1);
        }
        if (t1.size() == 1) {
            t2.push_back(t1[0]);
        }
        t1 = t2;
    }
    return t1;
}

std::vector<Triangle> random_box_o() {
    float  x = (rand_val() * 8) - 4;
    float  y = (rand_val() * 8) - 4;
    float  z = rand_val() * 5;
    float scale = rand_val() * 2;
    return Obj::make_box(glm::vec3(x, y, z), scale, rand_color());
}

BoundingBox random_box() {
    float  x = (rand_val() * 8) - 4;
    float  y = (rand_val() * 8) - 4;
    float  z = rand_val() * 5;
    float scale = rand_val() * 2;
    BoundingBox bb = BoundingBox(glm::vec3(x, y, z), scale);
    std::vector<Triangle> box = Obj::make_box(glm::vec3(x, y, z), scale, rand_color());
    bb.addChildObj(box);
    return bb;
}


int main(int argc, char** argv) {

    // set the number of boxes
    int NUM_BOXES = 2;

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(640, 480);

    // setup the camera
    float dist_to_origin = 5;
    Camera camera(
            glm::vec3(0, 0, -dist_to_origin),   // eye
            glm::vec3(0, 0, 0),                 // target
            glm::vec3(0, 1, 0),                 // up
            glm::vec2(-5, -5),                  // viewport min
            glm::vec2(5, 5),                    // viewport max
            dist_to_origin,                     // distance from eye to view plane
            glm::vec3(.3, .6, .8)               // background color
    );

    // setup lights
    // see http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    // for good attenuation value.
    // I found the values at 7 to be nice looking
    PointLight l1(glm::vec3(1, 1, 1), glm::vec3(3, -3, 0), 1.0, .7, 0.18);
    DirectionalLight l2(glm::vec3(.5, .5, .5), glm::vec3(-5, 4, -1));
    Lights lights = { &l1, &l2 };

    // setup world
    World world;
    std::vector<BoundingBox> boxes1;
    std::vector<BoundingBox> boxes2;

    // add the light
    world.append(Sphere(l1.position(), .25, glm::vec3(1,1,1)));
    addSphere(boxes1, Sphere(l1.position(), .25, glm::vec3(1, 1, 1)));
    addSphere(boxes2, Sphere(l1.position(), .25, glm::vec3(1, 1, 1)));

    // and the spheres
    world.append(Sphere(glm::vec3(1, 1, 1), 1, rand_color()));
    world.append(Sphere(glm::vec3(2, 2, 4), 2, rand_color()));
    world.append(Sphere(glm::vec3(3, 3, 6), 3, rand_color()));
    addSphere(boxes1, Sphere(glm::vec3(1, 1, 1), 1, rand_color()));
    addSphere(boxes1, Sphere(glm::vec3(2, 2, 4), 2, rand_color()));
    addSphere(boxes1, Sphere(glm::vec3(3, 3, 6), 3, rand_color()));
    addSphere(boxes2, Sphere(glm::vec3(1, 1, 1), 1, rand_color()));
    addSphere(boxes2, Sphere(glm::vec3(2, 2, 4), 2, rand_color()));
    addSphere(boxes2, Sphere(glm::vec3(3, 3, 6), 3, rand_color()));

    // and add some boxes and prep world for rendering
    for (int i = 0 ; i < NUM_BOXES ; ++i) {
        world.append(random_box_o());
        boxes1.push_back(random_box());
        boxes2.push_back(random_box());
    }

    world.lock();
    for (int i = 0; i < boxes1.size(); i++) {
        boxes1[i].boxWorld.lock();
    }
    for (int i = 0; i < boxes2.size(); i++) {
        boxes2[i].boxWorld.lock();
    }

    std::vector<BoundingBox> sortedBoxes = processBoundingBoxes(boxes2);

    // create the intersector
    BruteForceIntersector intersector0;
    MySlickIntersector intersector1;
    MySlickIntersector2 intersector2;

    // and setup the renderer
    Renderer renderer0(&intersector0);
    Renderer renderer1(&intersector1);
    Renderer renderer2(&intersector2);


    // render
    Timer timer0, timer1, timer2;

    std::cout << "Rendering 3 images each with 4 spheres and " << NUM_BOXES << " boxes" << std::endl;

    timer0.start();
    renderer0.render(image, camera, lights, world);
    timer0.stop();
    image.save_image("ray-traced0.bmp");
    std::cout << "Image 0 rendered in " << timer0.total() << " milliseconds" << std::endl;

    timer1.start();
    renderer1.render(image, camera, lights, boxes1);
    timer1.stop();
    image.save_image("ray-traced1.bmp");
    std::cout << "Image 1 rendered in " << timer1.total() << " milliseconds" << std::endl;

    timer2.start();
    renderer2.render(image, camera, lights, sortedBoxes);
    timer2.stop();
    image.save_image("ray-traced2.bmp");
    std::cout << "Image 2 rendered in " << timer2.total() << " milliseconds" << std::endl;
}
