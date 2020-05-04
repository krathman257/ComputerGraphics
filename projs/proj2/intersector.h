#ifndef _CSCI441_INTERSECTOR_H_
#define _CSCI441_INTERSECTOR_H_

#include <vector>

#include "ray.h"
#include "shape.h"


class Intersector {
public:
    virtual ~Intersector() { }

    virtual Hit find_first_intersection(const std::vector<BoundingBox>& bb, const Ray& ray, const int& ttl) = 0;
};


#endif
