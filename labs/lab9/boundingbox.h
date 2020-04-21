#ifndef _CSCI441_BOUNDINGBOX_H_
#define _CSCI441_BOUNDINGBOX_H_

#include <vector>
#include <glm/glm.hpp>
#include <math.h>
#include "world.h"

struct BoundingBox{

    World boxWorld;
    glm::vec3 lbn;
    glm::vec3 rtf;
    glm::vec3 center;
    
    std::vector<BoundingBox> childBoxes;

    BoundingBox() {
        lbn = glm::vec3(0, 0, 0);
        rtf = glm::vec3(0, 0, 0);
        center = glm::vec3(0, 0, 0);
    }

    BoundingBox(glm::vec3 c, float s) {
        lbn = c + s * glm::vec3(-0.5f, -0.5f, -0.5f);
        rtf = c + s * glm::vec3(0.5f, 0.5f, 0.5f);
        center = calcCenter();
    }

    BoundingBox(BoundingBox b1, BoundingBox b2) {
        lbn = glm::vec3(std::min(b1.lbn.x, b2.lbn.x),
            std::min(b1.lbn.y, b2.lbn.y),
            std::min(b1.lbn.z, b2.lbn.z));
        rtf = glm::vec3(std::max(b1.rtf.x, b2.rtf.x),
            std::max(b1.rtf.y, b2.rtf.y),
            std::max(b1.rtf.z, b2.rtf.z));
        center = calcCenter();

        childBoxes.push_back(b1);
        childBoxes.push_back(b2);

        boxWorld.lock();
    }

    glm::vec3 calcCenter() {
        return glm::vec3((lbn.x + rtf.x) / 2.0f,
            (lbn.y + rtf.y) / 2.0f,
            (lbn.z + rtf.z) / 2.0f);
    }

    void addChildSphere(const Sphere& s) { 
        boxWorld.append(s); 
    }

    void addChildObj(std::vector<Triangle>& s) {
        for (int i = 0; i < s.size(); i++) {
            boxWorld.append(s[i]);
        }
    }

    float distance(BoundingBox b2) {
        float x = pow(center.x - b2.center.x, 2);
        float y = pow(center.y - b2.center.y, 2);
        float z = pow(center.z - b2.center.z, 2);
        return sqrt(abs(x + y + z));
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
