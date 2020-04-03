#ifndef _CSCI441_SHAPE_H_
#define _CSCI441_SHAPE_H_

#include <cstdlib>
#include <vector>
#include <set>

#include "csci441/Vector3.h"

const float M_PI = 3.141592653589793;

template <typename T, typename N, typename TEXT>
void add_vertex(T& coords,
        const N& x, const N& y, const N& z,
        const Vector4& n, 
        const TEXT& t1, const TEXT& t2) {
    coords.push_back(x);
    coords.push_back(y);
    coords.push_back(z);

    Vector4 normal = n.normalized();
    coords.push_back(normal.x());
    coords.push_back(normal.y());
    coords.push_back(normal.z());

    coords.push_back(t1);
    coords.push_back(t2);
}

template <typename T>
void printCoords(T& coords) {
    for (int i = 0; i < coords.size(); i++) {
        std::cout << coords[i];
        if (i % 9 == 0) {
            std::cout << std::endl;
        }
        else {
            std::cout << ", ";
        }
    }
}

class CustomObj {
public:
    std::vector<float> coords;
    CustomObj(std::vector<std::vector<float>> v, std::vector<Vector3> t, float tM, bool isFlat) {
        bool textSwitch = false;
        for (int i = 0; i < t.size(); i++) {
            int ind1 = int(t[i].v[0])-1;
            int ind2 = int(t[i].v[1])-1;
            int ind3 = int(t[i].v[2])-1;
            Vector4 normal1, normal2, normal3;
            if (isFlat) {
                Vector4 point1 = Vector4(v[ind1][0], v[ind1][1], v[ind1][2]);
                Vector4 point2 = Vector4(v[ind2][0], v[ind2][1], v[ind2][2]);
                Vector4 point3 = Vector4(v[ind3][0], v[ind3][1], v[ind3][2]);

                Vector4 vec1 = point1 - point2;
                Vector4 vec2 = point1 - point3;
                normal1 = vec1.cross(vec2);
                normal2 = normal1;
                normal3 = normal1;
            }
            else {
                normal1 = Vector4(v[ind1][6], v[ind1][7], v[ind1][8]);
                normal2 = Vector4(v[ind2][6], v[ind2][7], v[ind2][8]);
                normal3 = Vector4(v[ind3][6], v[ind3][7], v[ind3][8]);
            }
            if (textSwitch) {
                add_vertex(coords, v[ind1][0], v[ind1][1], v[ind1][2], normal1, 0.0f, 0.0f);
                add_vertex(coords, v[ind2][0], v[ind2][1], v[ind2][2], normal2, 0.0f, tM);
                add_vertex(coords, v[ind3][0], v[ind3][1], v[ind3][2], normal3, tM, tM);
            }
            else {
                add_vertex(coords, v[ind1][0], v[ind1][1], v[ind1][2], normal1, 0.0f, 0.0f);
                add_vertex(coords, v[ind2][0], v[ind2][1], v[ind2][2], normal2, 0.0f, tM);
                add_vertex(coords, v[ind3][0], v[ind3][1], v[ind3][2], normal3, tM, tM);
            }
            textSwitch = !textSwitch;
        }
    }
};

#endif
