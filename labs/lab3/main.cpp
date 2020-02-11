#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "csci441/Shader.h"
#include "csci441/Vector3.h"
#include "csci441/Matrix3.h"

const float PI = 3.141592653589793;

bool modeSwitch = false;
int mode = 0;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        modeSwitch = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && modeSwitch) {
        mode = (mode + 1) % 5;
        modeSwitch = false;
    }
}

Matrix3 createStaticMatrix() {
    Matrix3 temp;
    temp.toIdent();
    return temp;
}

Matrix3 createRotateMatrix(float val) {
    Matrix3 temp;
    temp.toZRotation(val);
    return temp;
}

Matrix3 createOffCenterRotateMatrix(float xVal, float yVal, float rVal) {
    Matrix3 t_temp, r_temp;
    t_temp.toTranslate(xVal, yVal);
    r_temp.toZRotation(rVal);
    Matrix3 temp3 = t_temp.multiply(r_temp);
    return temp3;
}

Matrix3 createScaleMatrix(float val) {
    Matrix3 temp;
    temp.toScale(val);
    return temp;
}

Matrix3 createImpressiveMatrix(double xVal, double yVal, float width, float height, float sVal, float* rVal) {
    Matrix3 t_temp, r_temp, s_temp;
    xVal = -1.0 + xVal * (2.0 / width);
    yVal = 1.0 - yVal * (2.0 / height);
    *rVal += 0.002 * sqrt(xVal * xVal + yVal * yVal);
    t_temp.toTranslate(xVal / sVal, yVal / sVal);
    r_temp.toZRotation(*rVal * 2);
    s_temp.toScale(sVal);
    Matrix3 final_temp = t_temp.multiply(r_temp.multiply(s_temp));
    return final_temp;
}

int main(void) {

    /* Initialize the library */
    GLFWwindow* window;
    if (!glfwInit()) {
        return -1;
    }

#ifdef __APPLE__
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Lab 3", NULL, NULL);
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
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* init the triangle drawing */
    // define the vertex coordinates of the triangle
    float triangle[] = {
         0.5f,  0.5f, 1.0, 0.0, 0.0,
         0.5f, -0.5f, 1.0, 1.0, 0.0,
        -0.5f,  0.5f, 0.0, 1.0, 0.0,

         0.5f, -0.5f, 1.0, 1.0, 0.0,
        -0.5f, -0.5f, 0.0, 0.0, 1.0,
        -0.5f,  0.5f, 0.0, 1.0, 0.0,
    };

    // create and bind the vertex buffer object and copy the data to the buffer
    GLuint VBO[1];
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // create and bind the vertex array object and describe data layout
    GLuint VAO[1];
    glGenVertexArrays(1, VAO);
    glBindVertexArray(VAO[0]);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(0*sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    // create the shaders
    Shader shader("vert.glsl", "frag.glsl");

    float rotateVal = 0, scaleVal = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeVal = glfwGetTime();
        Matrix3 transMat;

        switch (mode) {

        case 0: 
            transMat.setArray(createStaticMatrix().m);
            break;

        case 1:
            rotateVal += 0.0005;
            transMat.setArray(createRotateMatrix(rotateVal).m);
            break;

        case 2:
            rotateVal += 0.0005;
            transMat.setArray(createOffCenterRotateMatrix(-0.5, 0.25, rotateVal).m);
            break;

        case 3:
            scaleVal = sin(timeVal) / 2.0f + 0.75f;
            transMat.setArray(createScaleMatrix(scaleVal).m);
            break;

        //Translate square to follow mouse cursor, fluxuate in size, rotate slower the closer it is to center
        case 4:
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            GLint v[4];
            glGetIntegerv(GL_VIEWPORT, v);
            scaleVal = sin(timeVal * 2) / 2.0f + 0.75f;
            transMat.setArray(createImpressiveMatrix(x, y, v[2], v[3], scaleVal, &rotateVal).m);
            break;

        default:
            break;
        }

        int transPosLoc = glGetUniformLocation(shader.ID, "transPos");
        glUniformMatrix3fv(transPosLoc, 1, true, transMat.m);

        // use the shader
        shader.use();

        // draw our triangles
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(triangle));

        /* Swap front and back * buffers */
        glfwSwapBuffers(window);

        /* Poll for and * process * events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
