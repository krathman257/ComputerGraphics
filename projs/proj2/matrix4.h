#ifndef _CSCI441_MATRIX4_H_
#define _CSCI441_MATRIX4_H_

#include <glm/glm.hpp>
#include <sstream>


class Matrix4 {
private:
    unsigned int idx(unsigned int r, unsigned int c) const {
        return r + c*4;
    }
    const float M_PI = 3.141592653589793;

public:
    float values[16];

    Matrix4() {
        set_to_identity();
    };

    float operator()(int row, int col) const {
        return values[idx(row, col)];
    }

    void set(int row, int col, float value) {
        values[idx(row, col)] = value;
    }

    void scale(float x, float y, float z) {
        set_to_identity();
        values[idx(0,0)] = x;
        values[idx(1,1)] = y;
        values[idx(2,2)] = z;
    }

    void rotate_x(float theta) {
        double theta_radians = theta*M_PI/180.0;

        set_to_identity();
        values[idx(1,1)] = std::cos(theta_radians);
        values[idx(1,2)] = -std::sin(theta_radians);
        values[idx(2,1)] = std::sin(theta_radians);
        values[idx(2,2)] = std::cos(theta_radians);
    }

    void rotate_y(float theta) {
        double theta_radians = theta*M_PI/180.0;

        set_to_identity();
        values[idx(0,0)] = std::cos(theta_radians);
        values[idx(0,2)] = std::sin(theta_radians);
        values[idx(2,0)] = -std::sin(theta_radians);
        values[idx(2,2)] = std::cos(theta_radians);
    }

    void rotate_z(float theta) {
        double theta_radians = theta*M_PI/180.0;

        set_to_identity();
        values[idx(0,0)] = std::cos(theta_radians);
        values[idx(0,1)] = -std::sin(theta_radians);
        values[idx(1,0)] = std::sin(theta_radians);
        values[idx(1,1)] = std::cos(theta_radians);
    }

    void translate(float x, float y, float z) {
        set_to_identity();
        values[idx(0,3)] = x;
        values[idx(1,3)] = y;
        values[idx(2,3)] = z;
    }

    Matrix4 operator*(const Matrix4& m) const {
        Matrix4 ret;
        mult(ret.values, values, m.values);
        return ret;
    }

    glm::vec3 operator*(const glm::vec3& v) const {
        glm::vec4 conv = glm::vec4(v.x, v.y, v.z, 1), ret;
        for (int i = 0; i < 4; ++i) {
            ret[i] = dot(values, i, conv);
        }
        return ret;
    }

    void operator=(const Matrix4& m) {
        for (int i = 0; i < 16; i++) {
            values[i] = m.values[i];
        }
    }

    std::string to_string()  const {
        std::ostringstream os;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                os << values[idx(row, col)] << "\t\t";
            }
            os << std::endl << std::endl;
        }
        return os.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix4& m) {
        os << m.to_string();
        return os;
    }

private:

    double dot(const float* m1, unsigned int row, const float* m2, int col) const {
        double dot = 0;
        for (int i = 0; i < 4; ++i) {
            dot += m1[idx(row,i)]*m2[idx(i,col)];
        }
        return dot;
    }

    double dot(const float* m1, unsigned int row, glm::vec4 v2) const {
        double dot = 0;
        for (int i = 0; i < 4; ++i) {
            dot += m1[idx(row, i)] * v2[i];
        }
        return dot;
    }

    void mult(float* target, const float* const m1, const float* const m2) const {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                target[idx(i,j)] = dot(m1, i, m2, j);
            }
        }
    }

    void set_to_identity() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                values[idx(i,j)] = i==j;
            }
        }
    }
};

#endif
