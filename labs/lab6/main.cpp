#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "csci441/Shader.h"
#include "csci441/Matrix4.h"
#include "csci441/Matrix3.h"
#include "csci441/Vector4.h"
#include "csci441/uniform.h"

#include "shape.h"
#include "model.h"
#include "camera.h"
#include "renderer.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

const char *duckPath = "models\\duck.obj";
const char *vertPath = "vert.glsl";
const char *fragPath = "frag.glsl";

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
    // SWITCH MODE
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

// Opening OBJ file process modified from:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/#reading-the-file

std::vector<glm::vec3> vertices;
std::vector<glm::vec3> triangles;
std::vector<glm::vec3> justVertices;
std::vector<std::vector<float>> fullVertices;

bool readObjFile(const char* filePath) {
    std::vector<glm::vec3> temp_vertices;
    std::vector<unsigned int> vertexIndices;

    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        std::cout << "Unable to open OBJ file" << std::endl;
        return false;
    }

    while (true) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) {
            break;
        }
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            glm::vec3 triangle;
            fscanf(file, "%f %f %f\n", &triangle.x, &triangle.y, &triangle.z);
            triangles.push_back(triangle);            
        }
    }

    for (int i = 0; i < vertices.size(); i++) {
        float count = 1.0f;
        std::vector<float> fullVertex;
        fullVertex.push_back(vertices[i][0]);
        fullVertex.push_back(vertices[i][1]);
        fullVertex.push_back(vertices[i][2]);
        fullVertex.push_back(1.0f);
        fullVertex.push_back(1.0f);
        fullVertex.push_back(0.0f);
        
        glm::vec3 normalAverage, temp_normal;
        for (int t = 0; t < triangles.size(); t++) {
            glm::vec3 a = vertices[triangles[t].x-1];
            glm::vec3 b = vertices[triangles[t].y-1];
            glm::vec3 c = vertices[triangles[t].z-1];
            if (triangles[t].x-1 == i) {
                temp_normal = glm::cross(a - b, a - c);
                normalAverage += (temp_normal - normalAverage) / count++;
            }
            else if (triangles[t].y-1 == i) {
                temp_normal = glm::cross(b - c, b - a);
                normalAverage += (temp_normal - normalAverage) / count++;
            }
            else if (triangles[t].z-1 == i) {
                temp_normal = glm::cross(c - a, c - b);
                normalAverage += (temp_normal - normalAverage) / count++;
            }
        }
        glm::normalize(normalAverage);

        fullVertex.push_back(normalAverage.x);
        fullVertex.push_back(normalAverage.y);
        fullVertex.push_back(normalAverage.z);
        
        fullVertices.push_back(fullVertex);
    }
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
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CSCI441-lab", NULL, NULL);
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

    //Read in duck
    readObjFile(duckPath);
    
    Model objFlat(
        CustomObj(fullVertices, triangles, true).coords,
        Shader(vertPath, fragPath));
    objFlat.model.scale(0.001, 0.001, 0.001);
    
    Model objSmooth(
        CustomObj(fullVertices, triangles, false).coords,
        Shader(vertPath, fragPath));
    objSmooth.model.scale(0.001, 0.001, 0.001);

    // make a floor
    Model floor(
            DiscoCube().coords,
            Shader(vertPath, fragPath));
    Matrix4 floor_trans, floor_scale;
    floor_trans.translate(0, -2, 0);
    floor_scale.scale(100, 1, 100);
    floor.model = floor_trans*floor_scale;

    // setup camera
    Matrix4 projection;
    projection.perspective(45, 1, .01, 10);

    Camera camera;
    camera.projection = projection;
    camera.eye = Vector4(0, 0, 3);
    camera.origin = Vector4(0, 0, 0);
    camera.up = Vector4(0, 1, 0);

    // and use z-buffering
    glEnable(GL_DEPTH_TEST);

    // create a renderer
    Renderer renderer;

    // set the light position
    Vector4 lightPos(3.75f, 3.75f, 4.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(objFlat.model, window);
        processInput(objSmooth.model, window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the object and the floor
        switch (mode) {
        case 0:
            renderer.render(camera, objFlat, lightPos);
            break;
        case 1:
            renderer.render(camera, objSmooth, lightPos);
            break;
        default:
            break;
        }
        renderer.render(camera, floor, lightPos);

        /* Swap front and back and poll for io events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
