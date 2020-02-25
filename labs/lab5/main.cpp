#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "csci441/Shader.h"
#include "csci441/Matrix4.h"
#include "csci441/Vector4.h"
#include "csci441/Vector3.h"
#include "csci441/Uniform.h"
#include "csci441/Shape.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

bool modeSwitch = false;
int mode = 0;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool isPressed(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool isReleased(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_RELEASE;
}

Matrix4 processModel(const Matrix4& model, GLFWwindow *window) {
    Matrix4 trans;

    const float ROT = 1;
    const float SCALE = .05;
    const float TRANS = .025;

    // ROTATE
    if (isPressed(window, GLFW_KEY_U)) { trans.rotate_x(-ROT); }
    else if (isPressed(window, GLFW_KEY_I)) { trans.rotate_x(ROT); }
    else if (isPressed(window, GLFW_KEY_O)) { trans.rotate_y(-ROT); }
    else if (isPressed(window, GLFW_KEY_P)) { trans.rotate_y(ROT); }
    else if (isPressed(window, '[')) { trans.rotate_z(-ROT); }
    else if (isPressed(window, ']')) { trans.rotate_z(ROT); }
    // SCALE
    else if (isPressed(window, '-')) { trans.scale(1-SCALE, 1-SCALE, 1-SCALE); }
    else if (isPressed(window, '=')) { trans.scale(1+SCALE, 1+SCALE, 1+SCALE); }
    // TRANSLATE
    else if (isPressed(window, GLFW_KEY_UP)) { trans.translate(0, TRANS, 0); }
    else if (isPressed(window, GLFW_KEY_DOWN)) { trans.translate(0, -TRANS, 0); }
    else if (isPressed(window, GLFW_KEY_LEFT)) { trans.translate(TRANS, 0, 0); }
    else if (isPressed(window, GLFW_KEY_RIGHT)) { trans.translate(-TRANS, 0, 0); }
    else if (isPressed(window, ',')) { trans.translate(0,0,TRANS); }
    else if (isPressed(window, '.')) { trans.translate(0,0,-TRANS); }
    // MODE
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { modeSwitch = true; }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && modeSwitch) { mode = (mode + 1) % 2; modeSwitch = false; }

    return trans * model;
}

void processInput(Matrix4& model, GLFWwindow *window) {
    if (isPressed(window, GLFW_KEY_ESCAPE) || isPressed(window, GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    }
    model = processModel(model, window);
}

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int main(void) {
    GLFWwindow* window;

    glfwSetErrorCallback(errorCallback);

    /* Initialize the library */
    if (!glfwInit()) { return -1; }

    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "lab5", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // tell glfw what to do on resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // init glad
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }


    // create c
    Cylinder cylinder(400, 1, .2, .4);
    // Cone c(20, 1, .2, .4);
    // Sphere c(20, .5, 1, .2, .4);
    // Torus c(20, .75, .25, 1, .2, .4);
    DiscoCube cube;

    // copy vertex data
    GLuint VBO_CYL;
    glGenBuffers(1, &VBO_CYL);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_CYL);
    glBufferData(GL_ARRAY_BUFFER, cylinder.coords.size()*sizeof(float),
            &cylinder.coords[0], GL_STATIC_DRAW);
    GLuint VAO_CYL;
    glGenVertexArrays(1, &VAO_CYL);
    glBindVertexArray(VAO_CYL);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
        (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
        (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint VBO_CUBE;
    glGenBuffers(1, &VBO_CUBE);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_CUBE);
    glBufferData(GL_ARRAY_BUFFER, cube.coords.size()*sizeof(float),
            &cube.coords[0], GL_STATIC_DRAW);
    GLuint VAO_CUBE;
    glGenVertexArrays(1, &VAO_CUBE);
    glBindVertexArray(VAO_CUBE);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
        (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
        (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // setup projection
    Matrix4 projection;
    projection.perspective(45, 1, .01, 10);

    // setup view
    Vector4 eye(0, 0, -2);
    Vector4 origin(0, 0, 0);
    Vector4 up(0, 1, 0);

    Matrix4 camera;
    camera.look_at(eye, origin, up);

    // create the shaders
    Shader shader("vert.glsl", "frag.glsl");

    // setup the textures
    shader.use();

    // set the matrices
    Matrix4 model;
    Matrix3 normalTransform;
    Vector4 lightPosition(-3.0f, -3.0f, 3.0f, 1.0f);
    Vector4 lightColor(1.0f, 1.0f, 0.0f, 1.0f);
    float lightIntensity = 2,
          specular = 32,
          ambient = 0.2;

    // and use z-buffering
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(model, window);
        normalTransform.inverse_transpose(projection * camera * model);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        shader.use();

        Uniform::set(shader.id(), "model", model);
        Uniform::set(shader.id(), "projection", projection);
        Uniform::set(shader.id(), "camera", camera);
        Uniform::set(shader.id(), "normTransform", normalTransform);
        Uniform::set(shader.id(), "lightPos", lightPosition);
        Uniform::set(shader.id(), "lightColor", lightColor);
        Uniform::set(shader.id(), "lightIntensity", lightIntensity);
        Uniform::set(shader.id(), "specular", specular);
        Uniform::set(shader.id(), "ambient", ambient);

        float size;

        switch (mode) {
            case 0:
                glBindBuffer(GL_ARRAY_BUFFER, VBO_CUBE);
                size = cube.coords.size() * sizeof(float);
                glBindVertexArray(VAO_CUBE);
                break;
            case 1:
                glBindBuffer(GL_ARRAY_BUFFER, VBO_CYL);
                size = cylinder.coords.size() * sizeof(float);
                glBindVertexArray(VAO_CYL);
                break;
            default:
                break;
        }

        // render the cube
        
        glDrawArrays(GL_TRIANGLES, 0, size);

        /* Swap front and back and poll for io events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
