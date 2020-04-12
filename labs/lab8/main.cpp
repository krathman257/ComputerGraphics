#include <iostream>
#include <glm/glm.hpp>
#include <bitmap/bitmap_image.hpp>

const float MAXVAL = std::numeric_limits<float>::max();

struct Viewport {
    glm::vec2 min;
    glm::vec2 max;

    Viewport(const glm::vec2& min, const glm::vec2& max)
        : min(min), max(max) {}
};

struct Sphere {
    int id;
    glm::vec3 origin;
    rgb_t color;
    float radius;

    Sphere(const glm::vec3& o=glm::vec3(0,0,0),
            float r=0,
            const glm::vec3& c=glm::vec3(0,0,0))
        : origin(o), radius(r) {
            color = make_colour(c.x*255, c.y*255, c.z*255);
            static int id_seed = 0;
            id = ++id_seed;
        }
};

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(const glm::vec3& origin = glm::vec3(0, 0, 0),
        const glm::vec3& direction = glm::vec3(0, 0, 0))
        : origin(origin), direction(normalize(direction)) {}

    float findNearestIntersection(Sphere s) {
        float a = dot(direction, direction);
        float b = dot(2.0f * direction, origin - s.origin);
        float c = dot(origin - s.origin, origin - s.origin) - s.radius * s.radius;

        float d = b * b - 4 * a * c;

        if (d < 0) {
            return MAXVAL;
        }
        else if (d == 0) {
            return (-1 * b) / (2 * a);
        }
        else if (d > 0) {
            float r1 = (-1 * b + sqrt(d)) / (2 * a);
            float r2 = (-1 * b - sqrt(d)) / (2 * a);
            return std::min(r1, r2);
        }
    }
};

void render(bitmap_image& image, const std::vector<Sphere>& world, int mode=0, float perspDist=1) {

    Viewport view = Viewport(glm::vec2(-5, -5), glm::vec2(5, 5));
    rgb_t background = make_colour(75, 156, 211);
    float w = image.width(), h = image.height();

    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            image.set_pixel(i, h-j-1, background);

            glm::vec3 o, d;
            float ui = view.min[0] + (view.max[0] - view.min[0]) * (i + 0.5f) / image.width();
            float vj = view.min[1] + (view.max[1] - view.min[1]) * (j + 0.5f) / image.height();

            switch (mode) {
            case 0:
                o = glm::vec3(ui, vj, 0);
                d = glm::vec3(0, 0, 1);
                break;

            case 1:
                o = glm::vec3(0, 0, -perspDist);
                d = glm::vec3(ui, vj, perspDist);
                break;

            default:
                break;
            }

            Ray r = Ray(o, d);
            
            float lowestT = MAXVAL;

            for (int k = 0; k < world.size(); k++) {
                Sphere tempSphere = world[k];
                float ni = r.findNearestIntersection(tempSphere);
                if (ni < lowestT) {
                    lowestT = ni;
                    image.set_pixel(i, h-j-1, tempSphere.color);
                }
            }
        }
    }
}

int main(int argc, char** argv) {

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(640, 480);

    // build world
    std::vector<Sphere> world = {
        Sphere(glm::vec3(0, 0, 1), 1, glm::vec3(1,1,0)),
        Sphere(glm::vec3(1, 1, 4), 2, glm::vec3(0,1,1)),
        Sphere(glm::vec3(2, 2, 6), 3, glm::vec3(1,0,1)),
    };

    // render the world
    render(image, world, 0);
    image.save_image("orthographic.bmp");

    render(image, world, 1, 5);
    image.save_image("perspective.bmp");

    std::cout << "Success" << std::endl;
}
