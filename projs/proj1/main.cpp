#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "csci441/Matrix4.h"
#include "csci441/Matrix3.h"
#include "csci441/Vector3.h"
#include "csci441/Vector4.h"
#include "csci441/uniform.h"
#include "csci441/Shader.h"

#include "camera.h"
#include "model.h"
#include "shape.h"
#include "renderer.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

//std::string projectPath = "C:\\Users\\Krathman257\\Desktop\\Homework\\Computer Graphics\\projs\\proj1\\";
std::string projectPath = "";
std::string rockPath = projectPath + "images\\stone.jpeg";
std::string skinPath = projectPath + "images\\skin.jpg";
std::string dinoPath = projectPath + "models\\dino.obj";
std::string mazePath = projectPath + "models\\maze.obj";
std::string vertPath = projectPath + "vert.glsl";
std::string fragPath = projectPath + "frag.glsl";

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

// Birds-eye controls
Matrix4 processModelBird(const Matrix4& model, GLFWwindow *window) {
    Matrix4 trans_x, trans_z;

    const float TRANS = .01;

    // FORWARD
    if (isPressed(window, GLFW_KEY_W)) { trans_z.translate(0, 0, -TRANS); }
    else if (isPressed(window, GLFW_KEY_S)) { trans_z.translate(0, 0, TRANS); }

    //STRAFE
    if (isPressed(window, GLFW_KEY_A)) { trans_x.translate(-TRANS, 0, 0); }
    else if (isPressed(window, GLFW_KEY_D)) { trans_x.translate(TRANS, 0, 0); }

    // SWITCH MODE
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { modeSwitch = true; }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && modeSwitch) { mode = (mode + 1) % 2; modeSwitch = false; }

    return trans_x * trans_z * model;
}

// First person controls
Matrix4 processModelPerson(const Matrix4& model, GLFWwindow* window) {
    Matrix4 trans_x, trans_z, rot, posCurr, negCurr, transRot;

    const float ROT = 0.25;
    const float TRANS = .01;

    posCurr.translate(model.values[12], 0, model.values[14]);
    negCurr.translate(-1 * model.values[12], 0, -1 * model.values[14]);

    // ROTATE
    if (isPressed(window, GLFW_KEY_D)) { rot.rotate_y(-ROT); }
    else if (isPressed(window, GLFW_KEY_A)) { rot.rotate_y(ROT); }

    // FORWARD
    if (isPressed(window, GLFW_KEY_W)) { 
        Vector4 t = model.getRotation() * Vector4(TRANS, 0, 0);
        trans_z.translate(t.values[0] * 0.5, 0, t.values[2] * 0.5);
    }
    else if (isPressed(window, GLFW_KEY_S)) { 
        transRot.rotate_y(180);
        Vector4 t = model.getRotation() * transRot * Vector4(TRANS, 0, 0);
        trans_z.translate(t.values[0] * 0.5, 0, t.values[2] * 0.5);
    }

    //STRAFE
    if (isPressed(window, GLFW_KEY_Q)) {
        transRot.rotate_y(90);
        Vector4 t = model.getRotation() * transRot * Vector4(TRANS, 0, 0);
        trans_x.translate(t.values[0] * 0.5, 0, t.values[2] * 0.5);
    }
    else if (isPressed(window, GLFW_KEY_E)) {
        transRot.rotate_y(270);
        Vector4 t = model.getRotation() * transRot * Vector4(TRANS, 0, 0);
        trans_x.translate(t.values[0] * 0.5, 0, t.values[2] * 0.5);
    }

    // SWITCH MODE
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { modeSwitch = true; }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && modeSwitch) { mode = (mode + 1) % 2; modeSwitch = false; }

    return trans_x * trans_z * posCurr * rot * negCurr * model;
}

// Choose between Bird or Person controls by mode
void processInput(Matrix4& model, GLFWwindow *window) {
    if (isPressed(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
    switch (mode) {
    case 0:
        model = processModelBird(model, window);
        break;
    case 1:
        model = processModelPerson(model, window);
        break;
    default:
        break;
    }
}

// Keep camera attached to player
void processCamera(Camera& camera, Matrix4& model, GLFWwindow* window) {
    Matrix4 posCurr, negCurr, correction;
    float originPitch = camera.origin.values[1], pitchLimit = 6.0f;

    posCurr.translate(camera.eye.values[0], 0, camera.eye.values[2]);
    negCurr.translate(-1 * (camera.eye.values[0]), 0, -1 * (camera.eye.values[2]));
    correction.rotate_y(-90);

    if (isPressed(window, GLFW_KEY_R)) { originPitch += 0.02; if (originPitch > pitchLimit) { originPitch = pitchLimit; } }
    else if (isPressed(window, GLFW_KEY_F)) { originPitch -= 0.02; if (originPitch < -pitchLimit) { originPitch = -pitchLimit; } }

    camera.eye = Vector4(model.values[12], camera.eye.values[1], model.values[14]);
    camera.origin = posCurr * correction * model.getRotation() * negCurr * Vector4(model.values[12], originPitch / 2.5, model.values[14] - 6);
}

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

// Opening OBJ file process modified from:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/#reading-the-file

bool readObjFile(const char* filePath, std::vector<std::vector<float>>* fv, std::vector<Vector3>* triangles) {
    std::vector<Vector3> vertices;

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
            Vector3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.v[0], &vertex.v[1], &vertex.v[2]);
            vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            Vector3 triangle;
            fscanf(file, "%f %f %f\n", &triangle.v[0], &triangle.v[1], &triangle.v[2]);
            triangles->push_back(triangle);            
        }
    }

    for (int i = 0; i < vertices.size(); i++) {
        float count = 1.0f;
        std::vector<float> fullVertex;
        fullVertex.push_back(vertices[i].v[0]);
        fullVertex.push_back(vertices[i].v[1]);
        fullVertex.push_back(vertices[i].v[2]);
        fullVertex.push_back(0.75f);
        fullVertex.push_back(0.75f);
        fullVertex.push_back(0.75f);
        
        Vector3 normalAverage, temp_normal;
        for (int t = 0; t < triangles->size(); t++) {
            Vector3 a = vertices[(*triangles)[t].v[0]-1];
            Vector3 b = vertices[(*triangles)[t].v[1]-1];
            Vector3 c = vertices[(*triangles)[t].v[2]-1];
            if ((*triangles)[t].v[0]-1 == i) {
                temp_normal = (a - b).crossProduct(a - c);
                normalAverage += (temp_normal - normalAverage) / count++;
            }
            else if ((*triangles)[t].v[1]-1 == i) {
                temp_normal = (b - c).crossProduct(b - a);
                normalAverage += (temp_normal - normalAverage) / count++;
            }
            else if ((*triangles)[t].v[2]-1 == i) {
                temp_normal = (c - a).crossProduct(c - b);
                normalAverage += (temp_normal - normalAverage) / count++;
            }
        }
        normalAverage = normalAverage.normalize();

        fullVertex.push_back(normalAverage.v[0]);
        fullVertex.push_back(normalAverage.v[1]);
        fullVertex.push_back(normalAverage.v[2]);
        
        fv->push_back(fullVertex);
    }
}

// Import model with specified position, rotation, color, scale, and shading model
Model importModel(std::string path, Vector3 position, Vector3 rotation, float tM, float scale, bool isFlat) {
    std::vector<Vector3> tempT;
    std::vector<std::vector<float>> tempV;
    readObjFile(path.c_str(), &tempV, &tempT);
    
    Model tempM(
        CustomObj(tempV, tempT, tM, isFlat).coords,
        Shader(vertPath, fragPath));

    Matrix4 rot_x, rot_y, rot_z, scal, tran;
    rot_x.rotate_x(rotation.v[0]);
    rot_y.rotate_y(rotation.v[1]);
    rot_z.rotate_z(rotation.v[2]);
    scal.scale(scale, scale, scale);
    tran.translate(position.v[0], position.v[1], position.v[2]);

    tempM.model = tran * scal * rot_x * rot_y * rot_z * tempM.model;

    return tempM;
}

// Load texture from file
GLuint loadTexture(const std::string& path, bool flip = true) {
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

    // Initialize window
    GLFWwindow* window;
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) { return -1; }
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CSCI441: A Maze Before Time", NULL, NULL);
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

    // Read in objects
    Model maze = importModel(
        mazePath,
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        2,
        6,
        true);

    Model dino = importModel(
        dinoPath,
        Vector3(-6.2f, 0.7f, -5.0f),
        Vector3(0.0f, 180.0f, 0.0f),
        1,
        2.5,
        false);

    // Set up cameras
    Matrix4 projection, orthographic;
    orthographic.ortho(-10, 10, -10, 10, -10, 10);
    projection.perspective(45, 1, .01, 20);

    Camera camera_bird, camera_person;

    camera_bird.projection = orthographic;
    camera_bird.eye = Vector4(0, 1, 0);
    camera_bird.origin = Vector4(0, 0, 0);
    camera_bird.up = Vector4(0, 0, -1);

    camera_person.projection = projection;
    camera_person.eye = Vector4(0.8, 1.5, 6);
    camera_person.origin = Vector4(0, -2, 0);
    camera_person.up = Vector4(0, 1, 0);

    //Set up textures
    glActiveTexture(GL_TEXTURE0);
    GLuint mazeTexture = loadTexture(rockPath, true);
    glBindTexture(GL_TEXTURE_2D, mazeTexture);

    glActiveTexture(GL_TEXTURE1);
    GLuint dinoTexture = loadTexture(skinPath, true);
    glBindTexture(GL_TEXTURE_2D, dinoTexture);

    // Use z-buffering
    glEnable(GL_DEPTH_TEST);

    // Create a renderer
    Renderer renderer;

    // Set the light position
    Vector4 lightPos(0.0f, 3.25f, 0.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        // process input
        processInput(dino.model, window);
        processCamera(camera_person, dino.model, window);

        // Set camera
        Camera current_camera;
        switch (mode) {
        case 0:
            current_camera = camera_bird;
            glClearColor(0.337f, 0.490f, 0.275f, 1.0f);
            break;
        case 1:
            current_camera = camera_person;
            glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
            break;
        default:
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render objects
        Uniform::set(dino.shader.id(), "aText", 0);
        renderer.render(current_camera, dino, lightPos);

        Uniform::set(maze.shader.id(), "aText", 1);
        renderer.render(current_camera, maze, lightPos);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
