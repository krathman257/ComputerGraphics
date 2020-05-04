#ifndef _CSCI441_SHAPE_H_
#define _CSCI441_SHAPE_H_

#include "matrix4.h"
#include "ray.h"
#include "texture.h"

class Shape {

private:
    int _id;
    int next_id() {
        static int id_seed = 0;
        return ++id_seed;
    }

public:
    Shape() : _id(next_id()) {}
    Shape(const Shape& s) : _id(s.id()) { }
    virtual ~Shape() {}

    int id() const {
        return _id;
    }

    double default_zero() const {
        return 0;
    }

    double default_inf() const {
        return std::numeric_limits<double>::infinity();
    }

    virtual glm::vec3 normal(const glm::vec3& p) const {
        return glm::vec3(0,0,0);
    }

    virtual glm::vec3 color(const glm::vec3& p) const {
        return glm::vec3(0,0,0);
    }

    virtual double intersect(const Ray& r) const {
        return intersect(r, default_zero(), default_inf());
    }

    virtual double intersect(const Ray& r, double zero, double inf) const {
        return inf;
    }

    virtual bool reflect() const {
        return false;
    }

    virtual std::string toString() const {
        return "Empty shape";
    }
};



struct Sphere : public Shape {
private:
    const float M_PI = 3.141592653589793;

    Texture& _texture;

    glm::vec3 _center;

    bool _reflect;
    float _radius;

public:
    Sphere(
        const glm::vec3& center,
        float radius,
        Texture& texture,
        bool reflect = false
    ) : Shape(), _center(center), _radius(radius), _texture(texture), _reflect(reflect) { }

    glm::vec3 normal(const glm::vec3& p) const {
        return glm::normalize(p-_center);
    }

    glm::vec3 color(const glm::vec3& p) const {
        glm::vec3 n = normal(-p);
        float u = atan2(n.x, n.z) / (2 * M_PI) + 0.5;
        float v = n.y * 0.5 + 0.5;
        return _texture.getColor(u, v);
    }

    glm::vec3 center() const { return _center; }
    float radius() const { return _radius; }
    float diameter() const { return _radius * 2.0f; }

    bool reflect() const {
        return _reflect;
    }

    std::string toString() const { 
        return "Sphere";
    }

    double intersect(const Ray& r) {
        return intersect(r, default_zero(), default_inf());
    }

    double intersect(const Ray& r, double zero, double inf) const {
        double t = inf;

        glm::vec3 l = _center - r.origin;

        double t_ca = glm::dot(l, r.direction);

        double d2 = glm::dot(l, l) - t_ca*t_ca;
        double r2 = _radius*_radius;
        if (t_ca >= 0 && d2 <= r2) {
            double t_hc = sqrt(r2 - d2);
            double t0 = t_ca - t_hc;
            double t1 = t_ca + t_hc;

            if (t0 > t1) {
                std::swap(t0, t1);
            }

            if (t0 > zero) {
                t = t0;
            } else if (t1 > zero) {
                t = t1;
            }
        }
        return t;
    }
};


struct Triangle : public Shape {
private:
    const float EPSILON = 0.0000001;
    float _area;

    bool _isFlat;
    bool _reflect;
    
    glm::vec3 _a, _aNorm,
            _b, _bNorm,
            _c, _cNorm, 
            _e1, _e2, _color;

public:
    Triangle(
        const glm::vec3& a,
        const glm::vec3& b,
        const glm::vec3& c,
        const glm::vec3& color = glm::vec3(0, 0, 0),
        bool isflat = true,
        bool reflect = false,
        const glm::vec3& an = glm::vec3(0, 0, 0),
        const glm::vec3& bn = glm::vec3(0, 0, 0),
        const glm::vec3& cn = glm::vec3(0, 0, 0)
        
    ) : Shape(), _a(a), _b(b), _c(c), _color(color), _isFlat(isflat), _reflect(reflect), _aNorm(an), _bNorm(bn), _cNorm(cn)  {
        _area = length(cross(_a - _b, _a - _c));
        _e1 = _b - _a;
        _e2 = _c - _a;
    }

    glm::vec3 a() const { return _a; }
    glm::vec3 b() const { return _b; }
    glm::vec3 c() const { return _c; }

    glm::vec3 normal(const glm::vec3& p) const {
        if (!_isFlat) {
            float a1 = length(cross(_b - p, _c - p)) / _area;
            float a2 = length(cross(_a - p, _c - p)) / _area;
            float a3 = 1 - a1 - a2;
            return glm::normalize(_aNorm * a1 + _bNorm * a2 + _cNorm * a3);
        }
        else {
            return glm::normalize(glm::cross(_b - _a, _c - _a));
        }
    }

    glm::vec3 color(const glm::vec3&) const {
        return _color;
    }

    bool reflect() const {
        return _reflect;
    }

    std::string toString() const {
        return "Triangle";
    }

    double intersect(const Ray& r) {
        return intersect(r, default_zero(), default_inf());
    }

    double intersect(const Ray& r, double zero, double inf) const {
        glm::vec3 h, s, q;
        float a, f, u, v;
        h = cross(r.direction, _e2);
        a = dot(_e1, h);
        if (a > -EPSILON && a < EPSILON) { return inf; }
        f = 1.0 / a;
        s = r.origin - _a;
        u = f * dot(s, h);
        if (u < 0.0 || u > 1.0) { return inf; }
        q = cross(s, _e1);
        v = f * dot(r.direction, q);
        if (v < 0.0 || u + v > 1.0) { return inf; }
        float t = f * dot(_e2, q);
        if (t > EPSILON) { return t; }
        else { return inf; }
    }
};

struct ImportedObj {

    static std::vector<Triangle> import_obj(
        std::string filePath,
        glm::vec3 position = glm::vec3(0, 0, 0),
        glm::vec3 rotation = glm::vec3(0, 0, 0),
        glm::vec3 color = glm::vec3(0, 0, 0),
        float scale = 1,
        bool isFlat = false,
        bool reflect = false
    ) {
        std::vector<std::vector<float>> fv;
        std::vector<glm::vec3> vertices, triangles;
        std::vector<Triangle> polygons;

        FILE* file;
        errno_t err = fopen_s(&file, filePath.c_str(), "r");
        if (err != 0) {
            std::cout << "Unable to open OBJ file" << std::endl;
            return std::vector<Triangle>();
        }

        while (true) {
            char lineHeader[128];
            int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
            if (res == EOF) {
                break;
            }
            if (strcmp(lineHeader, "v") == 0) {
                glm::vec3 vertex;
                fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                vertices.push_back(vertex);
            }
            else if (strcmp(lineHeader, "f") == 0) {
                glm::vec3 triangle;
                fscanf_s(file, "%f %f %f\n", &triangle.x, &triangle.y, &triangle.z);
                triangles.push_back(triangle);
            }
        }

        Matrix4 trans, rot_x, rot_y, rot_z, scal, complete;

        trans.translate(position.x, position.y, position.z);
        rot_x.rotate_x(rotation.x);
        rot_y.rotate_y(rotation.y);
        rot_z.rotate_z(rotation.z);
        scal.scale(scale, scale, scale);

        complete = trans * rot_x * rot_y * rot_z * scal;

        for (int i = 0; i < vertices.size(); i++) {
            float count = 1.0f;
            std::vector<float> fullVertex;
            fullVertex.push_back(vertices[i].x);
            fullVertex.push_back(vertices[i].y);
            fullVertex.push_back(vertices[i].z);

            glm::vec3 normalAverage, temp_normal;
            for (int t = 0; t < triangles.size(); t++) {
                glm::vec3 a = vertices[triangles[t].x - 1];
                glm::vec3 b = vertices[triangles[t].y - 1];
                glm::vec3 c = vertices[triangles[t].z - 1];
                if (triangles[t].x - 1 == i) {
                    temp_normal = cross(a - b, a - c);
                    normalAverage += (temp_normal - normalAverage) / count++;
                }
                else if (triangles[t].y - 1 == i) {
                    temp_normal = cross(b - c, b - a);
                    normalAverage += (temp_normal - normalAverage) / count++;
                }
                else if (triangles[t].z - 1 == i) {
                    temp_normal = cross(c - a, c - b);
                    normalAverage += (temp_normal - normalAverage) / count++;
                }
            }
            normalAverage = normalize(normalAverage);

            fullVertex.push_back(normalAverage.x);
            fullVertex.push_back(normalAverage.y);
            fullVertex.push_back(normalAverage.z);

            fv.push_back(fullVertex);
        }

        for (int t = 0; t < triangles.size(); t++) {
            int vert1 = triangles[t].x - 1,
                vert2 = triangles[t].y - 1,
                vert3 = triangles[t].z - 1;
            glm::vec3 a = complete * glm::vec3(fv[vert1][0], fv[vert1][1], fv[vert1][2]);
            glm::vec3 b = complete * glm::vec3(fv[vert2][0], fv[vert2][1], fv[vert2][2]);
            glm::vec3 c = complete * glm::vec3(fv[vert3][0], fv[vert3][1], fv[vert3][2]);
            glm::vec3 an = normalize(complete * glm::vec3(fv[vert1][3], fv[vert1][4], fv[vert1][5]));
            glm::vec3 bn = normalize(complete * glm::vec3(fv[vert2][3], fv[vert2][4], fv[vert2][5]));
            glm::vec3 cn = normalize(complete * glm::vec3(fv[vert3][3], fv[vert3][4], fv[vert3][5]));

            polygons.push_back(Triangle(a, b, c, color, isFlat, reflect, an, bn, cn));
        }

        return polygons;
    }
};

#endif
