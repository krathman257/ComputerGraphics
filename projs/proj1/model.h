#ifndef _CSCI441_MODEL_H_
#define _CSCI441_MODEL_H_

#include "csci441/vector3.h"

class Model {

public:
    GLuint vbo;
    GLuint vao;
    Shader shader;
    Matrix4 model;

    int size;

    template <typename Coords>
    Model(const Coords& coords, const Shader& shader_in) : shader(shader_in) {
        size = coords.size()*sizeof(float);

        // copy vertex data
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, coords.data(), GL_STATIC_DRAW);

        // describe vertex layout
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                (void*)(0*sizeof(float)));
        glEnableVertexAttribArray(0);

        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);
    }
};

#endif
