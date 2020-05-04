#ifndef _CSCI441_TEXTURE_H_
#define _CSCI441_TEXTURE_H_

#include <random>
#include <chrono>

class Texture {
public:
    const float M_PI = 3.141592653589793;

    Texture() { }

    float perlin(float x, float y, std::vector<int> p) const {
        int X = (int)floor(x) % 255;
        int Y = (int)floor(y) % 255;
        float xf = x - floor(x);
        float yf = y - floor(y);

        glm::vec2 point_tr = glm::vec2(xf - 1.0, yf - 1.0);
        glm::vec2 point_tl = glm::vec2(xf, yf - 1.0);
        glm::vec2 point_br = glm::vec2(xf - 1.0, yf);
        glm::vec2 point_bl = glm::vec2(xf, yf);

        glm::vec2 const_tr = constantVector(p[p[X + 1] + Y + 1]);
        glm::vec2 const_tl = constantVector(p[p[X] + Y + 1]);
        glm::vec2 const_br = constantVector(p[p[X + 1] + Y]);
        glm::vec2 const_bl = constantVector(p[p[X] + Y]);

        float dot_tr = dot(point_tr, const_tr);
        float dot_tl = dot(point_tl, const_tl);
        float dot_br = dot(point_br, const_br);
        float dot_bl = dot(point_bl, const_bl);

        float inter_r = interpolate(fade(yf), dot_br, dot_tr);
        float inter_l = interpolate(fade(yf), dot_bl, dot_tl);

        return (interpolate(fade(xf), inter_l, inter_r) + 1) / 2;
    }

    void generatePerm(std::vector<int>& p) {
        for (int i = 0; i < 256; i++) {
            p.push_back(i);
        }

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(p.begin(), p.end(), std::default_random_engine(seed));

        for (int i = 0; i < 256; i++) {
            p.push_back(p[i]);
        }
    }

    glm::vec2 constantVector(int v) const {
        int i = v % 4;
        if (i == 0) { return glm::vec2(1, 1); }
        else if (i == 1) { return glm::vec2(-1, 1); }
        else if (i == 2) { return glm::vec2(1, -1); }
        else { return glm::vec2(-1, -1); }
    }

    float interpolate(float t, float a, float b) const {
        return a + t * (b - a);
    }

    float fade(float t) const {
        return ((6 * t - 15) * t + 10) * t * t * t;
    }

    virtual glm::vec3 getColor(float u, float v) const {
        return glm::vec3(0, 0, 0);
    }
};

struct OneColorTexture : public Texture {
private:
    std::vector<int> perm;
    glm::vec3 color;
    float width, height;

public:
    OneColorTexture(
        float r,
        glm::vec3 c
    ) : Texture(), width(r * M_PI), height(r), color(c) { generatePerm(perm); }

    OneColorTexture(
        float w,
        float h,
        glm::vec3 c
    ) : Texture(), width(w), height(h), color(c) { generatePerm(perm); }

    glm::vec3 getColor(float u, float v) const {
        float row = u * width;
        float col = v * height;
        return color * perlin(row, col, perm);
    }
};

struct TwoColorTexture : public Texture {
private:
    std::vector<int> perm;
    glm::vec3 color1, color2;
    float width, height;

public:
    TwoColorTexture(
        float r,
        glm::vec3 c1,
        glm::vec3 c2
    ) : Texture(), width(r * M_PI), height(r), color1(c1), color2(c2) { generatePerm(perm); }

    TwoColorTexture(
        float w,
        float h,
        glm::vec3 c1,
        glm::vec3 c2
    ) : Texture(), width(w), height(h), color1(c1), color2(c2) { generatePerm(perm); }

    glm::vec3 getColor(float u, float v) const {
        float row = u * width;
        float col = v * height;
        float p = perlin(row, col, perm);
        return p * color1 + (1 - p) * color2;
    }
};

struct MixedTexture : public Texture {
private:
    std::vector<int> perm;
    Texture* texture1;
    Texture* texture2;
    float width, height, threshold;

public:
    MixedTexture(
        float r,
        float th,
        Texture& t1,
        Texture& t2
    ) : Texture(), width(r * M_PI), height(r), threshold(th), texture1(&t1), texture2(&t2) { generatePerm(perm); }

    MixedTexture(
        float w,
        float h,
        float th,
        Texture& t1,
        Texture& t2
    ) : Texture(), width(w), height(h), threshold(th), texture1(&t1), texture2(&t2) { generatePerm(perm); }

    glm::vec3 getColor(float u, float v) const {
        float row = u * width;
        float col = v * height;
        if (perlin(row, col, perm) > threshold) {
            return texture1->getColor(u, v);
        }
        return texture2->getColor(u, v);
    }
};

#endif