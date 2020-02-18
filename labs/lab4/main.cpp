#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "csci441/Shader.h"
#include "csci441/Matrix4.h"
#include "csci441/Vector4.h"
#include "csci441/Cube.h"
#include "csci441/Cylinder.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool modeSwitch_p = false;
bool modeSwitch_o = false;
bool perspective = false;
int objMode = 0;

float posX = 0,
      posY = 0,
      posZ = -2,
      scal = 1,
      rotX = 0,
      rotY = 0,
      rotZ = 0;

float camPosStep = 0.05,
    posStep = 0.05,
    scalStep = 0.05,
    rotStep = 0.05;
int sleepTime = 1;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void sleep(int s) {
    std::this_thread::sleep_for(std::chrono::milliseconds(s));
}

void processInput(GLFWwindow *window, Shader &shader) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        modeSwitch_o = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && modeSwitch_o) {
        objMode = (objMode + 1) % 2;
        modeSwitch_o = false;
    }
    if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS) {
        modeSwitch_p = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_RELEASE && modeSwitch_p) {
        perspective = !perspective;
        modeSwitch_p = false;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        posY -= posStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        posY += posStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        posY -= posStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        posY += posStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        posX += posStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        posX -= posStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        posZ += posStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
        posZ -= posStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        rotX += rotStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        rotX -= rotStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        rotY += rotStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        rotY -= rotStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
        rotZ += rotStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
        rotZ -= rotStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        scal += scalStep;
        sleep(sleepTime);
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        scal -= scalStep;
        if (scal < 0.05) {
            scal = 0.05;
        }
        sleep(sleepTime);
    }
}

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int main(void) {
    GLFWwindow* window;
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) { return -1; }

#ifdef __APPLE__
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 4", NULL, NULL);
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

    Cube cube1 = Cube(0, 0, 0.5);
    Cylinder cylinder1 = Cylinder(0, 0, 1.5, 0.5);

    GLuint VBO_CUBE;
    glGenBuffers(1, &VBO_CUBE);
    GLuint VBO_CYL;
    glGenBuffers(1, &VBO_CYL);

    Shader shader("vert.glsl", "frag.glsl");
    shader.use();

    glEnable(GL_DEPTH_TEST);

    Matrix4 cam_M, scal_M, ortho_M, pers_M, ident_M;

    ortho_M.toOrth(-2, 2, -2, 2, -6, 6);
    pers_M.toPerspective(1, 1.01, 0.3);
    ident_M.toIdent();

    while (!glfwWindowShouldClose(window)) {
        processInput(window, shader);

        switch (objMode) {
            case 0:
                glBindBuffer(GL_ARRAY_BUFFER, VBO_CUBE);
                glBufferData(GL_ARRAY_BUFFER, sizeof(cube1.v), cube1.v, GL_STATIC_DRAW);
                break;

            case 1:
                glBindBuffer(GL_ARRAY_BUFFER, VBO_CYL);
                glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder1.v), cylinder1.v, GL_STATIC_DRAW);
                break;

            default:
                break;
        }

        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scal_M.toScale(scal);

        cam_M.toCam(
            Vector3(posX, posY, posZ),
            Vector3(rotX, rotY, rotZ),
            Vector3(0, 0, 0)
        );

        int scalMatLoc = glGetUniformLocation(shader.ID, "scalMat");
        int camMatLoc = glGetUniformLocation(shader.ID, "camMat");
        int orthoMatLoc = glGetUniformLocation(shader.ID, "orthoMat");
        int persMatLoc = glGetUniformLocation(shader.ID, "persMat");

        glUniformMatrix4fv(scalMatLoc, 1, true, scal_M.m);
        glUniformMatrix4fv(camMatLoc, 1, true, cam_M.m);
        glUniformMatrix4fv(orthoMatLoc, 1, true, ortho_M.m);
        if (perspective) {
            glUniformMatrix4fv(persMatLoc, 1, true, pers_M.m);
        }
        else {
            glUniformMatrix4fv(persMatLoc, 1, true, ident_M.m);
        }

        shader.use();

        glBindVertexArray(VAO);
        GLsizei size;

        switch (objMode) {
        case 0:
            size = sizeof(cube1.v);
            break;

        case 1:
            size = sizeof(cylinder1.v);
            break;

        default:
            break;
        }
        glDrawArrays(GL_TRIANGLES, 0, size);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
