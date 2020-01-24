#include <iostream>

#include "bitmap_image.hpp"

class Point {
public:
    float x;
    float y;
    float r;
    float g;
    float b;

    Point() : x(0), y(0), r(0), g(0), b(0) {}

    Point(float xx, float yy) : x(xx), y(yy), r(0), g(0), b(0) {}

    float relativeCoord(Point a, Point b) {
        return ((a.y - b.y) * this->x) - ((a.x - b.x) * this->y) + (a.x * b.y) - (a.y * b.x);
    }

    float beta(Point pa, Point pb, Point pc) {
        float a = this->relativeCoord(pa, pc);
        float b = pb.relativeCoord(pa, pc);
        return a / b;
    }

    float gamma(Point pa, Point pb, Point pc) {
        float a = this->relativeCoord(pa, pb);
        float b = pc.relativeCoord(pa, pb);
        return a / b;
    }
};

float min(float a, float b, float c) {
    return std::min(a, std::min(b, c));
}

float max(float a, float b, float c) {
    return std::max(a, std::max(b, c));
}

bool ZeroToOne(float a) {
    return 0 < a && a < 1;
}

int main(int argc, char** argv) {
    std::cout << "Enter 3 points (enter a point as x y r g b):" << std::endl;

    Point p1, p2, p3;
    std::cin >> p1.x >> p1.y >> p1.r >> p1.g >> p1.b;
    std::cin >> p2.x >> p2.y >> p2.r >> p2.g >> p2.b;
    std::cin >> p3.x >> p3.y >> p3.r >> p3.g >> p3.b;

    bitmap_image image(640, 480);

    float bbxMin = min(p1.x, p2.x, p3.x);
    float bbxMax = max(p1.x, p2.x, p3.x);
    float bbyMin = min(p1.y, p2.y, p3.y);
    float bbyMax = max(p1.y, p2.y, p3.y);

    for (int x = bbxMin; x < bbxMax; x++) {
        for (int y = bbyMin; y < bbyMax; y++) {
            Point pt(x, y);
            
            float beta = pt.beta(p1, p2, p3);
            float gamma = pt.gamma(p1, p2, p3);
            float alpha = 1 - beta - gamma;

            if (ZeroToOne(alpha) && ZeroToOne(beta) && ZeroToOne(gamma)) {
                float red = alpha * p1.r + beta * p2.r + gamma * p3.r;
                float blue = alpha * p1.b + beta * p2.b + gamma * p3.b;
                float green = alpha * p1.g + beta * p2.g + gamma * p3.g;
                rgb_t tempColor = make_colour(red * 255, blue * 255, green * 255);
                image.set_pixel(x, y, tempColor);
            }
        }
    }

    image.save_image("triangle.bmp");
    std::cout << "Success" << std::endl;
}
