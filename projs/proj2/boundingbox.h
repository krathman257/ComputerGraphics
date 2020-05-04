#ifndef _CSCI441_BOUNDINGBOX_H_
#define _CSCI441_BOUNDINGBOX_H_

#include <vector>
#include <glm/glm.hpp>
#include <math.h>
#include "world.h"

struct BoundingBox{

    const float INF = FLT_MAX;

    World boxWorld;
    glm::vec3 lbn;
    glm::vec3 rtf;
    
    std::vector<BoundingBox> childBoxes;

    BoundingBox() {
        lbn = glm::vec3(0, 0, 0);
        rtf = glm::vec3(0, 0, 0);
    }

    BoundingBox(glm::vec3 c, float s) {
        lbn = c + s * glm::vec3(-0.5f, -0.5f, -0.5f);
        rtf = c + s * glm::vec3(0.5f, 0.5f, 0.5f);
    }

    BoundingBox(const Sphere& s) {
        lbn = s.center() + s.diameter() * glm::vec3(-0.5f, -0.5f, -0.5f);
        rtf = s.center() + s.diameter() * glm::vec3(0.5f, 0.5f, 0.5f);
        boxWorld.append(s);
    }

    BoundingBox(const std::vector<Triangle>& s) {
        lbn = glm::vec3(INF, INF, INF);
        rtf = glm::vec3(-INF, -INF, -INF);

        for (int i = 0; i < s.size(); i++) {

            adjustBounds(s[i].a());
            adjustBounds(s[i].b());
            adjustBounds(s[i].c());

            boxWorld.append(s[i]);
        }
    }

    void adjustBounds(glm::vec3 p) {
        if (p.x < lbn.x) { lbn.x = p.x; }
        if (p.x > rtf.x) { rtf.x = p.x; }

        if (p.y < lbn.y) { lbn.y = p.y; }
        if (p.y > rtf.y) { rtf.y = p.y; }

        if (p.z < lbn.z) { lbn.z = p.z; }
        if (p.z > rtf.z) { rtf.z = p.z; }
    }

    bool isHit(Ray ray) {
        glm::vec3 a;
        for (int i = 0; i < 3; i++) {
            if (ray.direction[i] != 0) {
                a[i] = 1 / ray.direction[i];
            }
            else {
                a[i] = 0;
            }
        }

        glm::vec2 tx, ty, tz;

        //X Boundries
        if (a[0] >= 0) {
            tx.x = a[0] * (lbn.x - ray.origin.x);
            tx.y = a[0] * (rtf.x - ray.origin.x);
        }
        else {
            tx.y = a[0] * (lbn.x - ray.origin.x);
            tx.x = a[0] * (rtf.x - ray.origin.x);
        }

        //Y Boundries
        if (a[1] >= 0) {
            ty.x = a[1] * (lbn.y - ray.origin.y);
            ty.y = a[1] * (rtf.y - ray.origin.y);
        }
        else {
            ty.y = a[1] * (lbn.y - ray.origin.y);
            ty.x = a[1] * (rtf.y - ray.origin.y);
        }

        //Z Boundries
        if (a[2] >= 0) {
            tz.x = a[2] * (lbn.z - ray.origin.z);
            tz.y = a[2] * (rtf.z - ray.origin.z);
        }
        else {
            tz.y = a[2] * (lbn.z - ray.origin.z);
            tz.x = a[2] * (rtf.z - ray.origin.z);
        }

        return !(tx.x > ty.y || ty.x > tx.y) &&
               !(tx.x > tz.y || tz.x > tx.y) &&
               !(ty.x > tz.y || tz.x > ty.y);
    }
};

#endif
