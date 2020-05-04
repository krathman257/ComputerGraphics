#ifndef _CSCI441_LIGHT_H_
#define _CSCI441_LIGHT_H_

class Light {
private:
    glm::vec3 _color;

public:

    Light(const glm::vec3& color=glm::vec3(1,1,1)) : _color(color) {}

    glm::vec3 color() const {
        return _color;
    }

    float attenuation(const glm::vec3& p) const {
        return 1;
    }

    virtual glm::vec3 direction(const glm::vec3& p) const = 0;
};


typedef std::vector<const Light*> Lights;


class DirectionalLight : public Light {
private:
    glm::vec3 _dir;

public:

    DirectionalLight(
        const glm::vec3& color=glm::vec3(1,1,1),
        const glm::vec3& dir=glm::vec3(0,0,0)
    ) : Light(color), _dir(dir) {}

    glm::vec3 direction(const glm::vec3&) const {
        return _dir;
    }
};


class PointLight : public Light {
private:
    glm::vec3 _pos;

    float _const;
    float _linear;
    float _quadratic;

public:

    PointLight(
        const glm::vec3& color=glm::vec3(1,1,1),
        const glm::vec3& pos=glm::vec3(0,0,0),
        float constant=1,
        float linear=.7,
        float quadratic=1.8
    ) : Light(color), _pos(pos), _const(constant),
        _linear(linear), _quadratic(quadratic) {}

    glm::vec3 direction(const glm::vec3& p) const {
        return _pos - p;
    }

    glm::vec3 position() const {
        return _pos;
    }

    float attenuation(const glm::vec3& p) const {
        float d = glm::length(_pos - p);
        float denom = _const + d*_linear + (d*d)*_quadratic;
        return 1.0 / denom;
    }
};


#endif
