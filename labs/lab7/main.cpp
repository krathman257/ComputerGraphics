#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "csci441/shader.h"
#include "csci441/matrix4.h"
#include "csci441/matrix3.h"
#include "csci441/model.h"
#include "csci441/renderer.h"
#include "csci441/vector4.h"
#include "csci441/uniform.h"

#include "shape.h"
#include "camera.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const std::string PATH = "";

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
    const float TRANS = .01;

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
    else if (isPressed(window, GLFW_KEY_LEFT)) { trans.translate(-TRANS, 0, 0); }
    else if (isPressed(window, GLFW_KEY_RIGHT)) { trans.translate(TRANS, 0, 0); }
    else if (isPressed(window, ',')) { trans.translate(0,0,TRANS); }
    else if (isPressed(window, '.')) { trans.translate(0,0,-TRANS); }

    return trans * model;
}

bool isSpaceEvent(GLFWwindow *window) {
    static bool pressed = false;

    bool trigger = false;
    if (isPressed(window, GLFW_KEY_SPACE)) {
        pressed = true;
    } else if (pressed && isReleased(window, GLFW_KEY_SPACE)) {
        pressed = false;
        trigger = true;
    }
    return trigger;
}

void processInput(Matrix4& model, GLFWwindow *window) {
    if (isPressed(window, GLFW_KEY_ESCAPE) || isPressed(window, GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    } else if (isSpaceEvent(window)) {
        mode = (mode + 1) % 4;
    }
    model = processModel(model, window);
}

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

GLuint createTexture() {
    GLuint textureID = 0;
    const int WIDTH = 250;
    const int HEIGHT = 250;
    const int numCheckers = 10;
    GLuint texture[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        int col = i % WIDTH;
        int row = (i - col) / HEIGHT;
        int checkWidth = WIDTH / numCheckers;
        int checkHeight = HEIGHT / numCheckers;
        bool color = false;
        if (col % (checkWidth * 2) == col % checkWidth) {
            color = !color;
        }
        if (row % (checkHeight * 2) == row % checkHeight) {
            color = !color;
        }
        if (color) {
            texture[i] = 0x00000000;
        }
        else {
            texture[i] = 0xFFFFFFFF;
        }
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;
}

GLuint loadTexture(const std::string& path, bool flip=true) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

    if (stbi_failure_reason() != NULL) {
        std::cout << "FAILURE: " << stbi_failure_reason() << std::endl;
    }

    if (data) {
        GLenum format = 0;
        switch (nrComponents) {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
            default: std::cout << "Incorrect number of components: " << nrComponents << std::endl;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}

int main(void) {
    GLFWwindow* window;

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) { return -1; }

    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CSCI441-lab", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    Matrix4 projection;
    projection.perspective(45, 1, .01, 10);

    Camera camera;
    camera.projection = projection;
    camera.eye = Vector4(0, 0, 3);
    camera.origin = Vector4(0, 0, 0);
    camera.up = Vector4(0, 1, 0);

    Shader shader(PATH + "vert.glsl", PATH + "frag.glsl");

    Model cube(Cube().getCoords(), shader);
    Model sphere(Sphere(20, 1).getCoords(), shader);

    glEnable(GL_DEPTH_TEST);

    Matrix4 model;

    glActiveTexture(GL_TEXTURE0);
    GLuint texture1 = createTexture();
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    GLuint texture2 = loadTexture(PATH + "img\\question.png", true);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glActiveTexture(GL_TEXTURE2);
    GLuint texture3 = loadTexture(PATH + "img\\giraffe-fur.jpg", false);
    glBindTexture(GL_TEXTURE_2D, texture3);

    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        processInput(cube.model, window);
        processInput(sphere.model, window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (mode < 2) {
            renderer.render(camera, cube);
        }
        else {
            renderer.render(camera, sphere);
        }
        if (mode % 2 == 0) {
            Uniform::set(shader.id(), "aText", 0);
        }
        else if (mode == 1) {
            Uniform::set(shader.id(), "aText", 1);
        }
        else {
            Uniform::set(shader.id(), "aText", 2);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
