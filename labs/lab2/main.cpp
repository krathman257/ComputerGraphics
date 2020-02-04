#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Point {
public:
    float x;
    float y;
    float r;
    float g;
    float b;

    Point() : x(0), y(0), r(0), g(0), b(0) {}
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* initWindow() {
    GLFWwindow* window;
    if (!glfwInit()) {
        return NULL;
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    window = glfwCreateWindow(640, 480, "Lab 2", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return NULL;
    }

    return window;
}

std::string shaderTypeName(GLenum shaderType) {
    switch(shaderType) {
        case GL_VERTEX_SHADER: return "VERTEX";
        case GL_FRAGMENT_SHADER: return "FRAGMENT";
        default: return "UNKNOWN";
    }
}

std::string readFile(const char* fileName) {
    std::ifstream stream(fileName);
    std::stringstream buffer;
    buffer << stream.rdbuf();

    std::string source = buffer.str();
    std::cout << "Source:" << std::endl;
    std::cout << source << std::endl;

    return source;
}

void w2nd(float* x, float* y) {
    *x = -1.0f + *x * (2.0f / 640.0f);
    *y = 1.0f - *y * (2.0f / 480.0f);
}

GLuint createShader(const char* fileName, GLenum shaderType) {
    std::string source = readFile(fileName);
    const char* src_ptr = source.c_str();

    GLuint shader = 0;

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src_ptr, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << shaderTypeName(shaderType)
            <<"::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = 0;

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    return program;
}

int main(void) {
    std::cout << "Enter 3 points (enter a point as x y r g b):" << std::endl;

    Point p1, p2, p3;

    std::cin >> p1.x >> p1.y >> p1.r >> p1.g >> p1.b;
    std::cin >> p2.x >> p2.y >> p2.r >> p2.g >> p2.b;
    std::cin >> p3.x >> p3.y >> p3.r >> p3.g >> p3.b;

    w2nd(&p1.x, &p1.y);
    w2nd(&p2.x, &p2.y);
    w2nd(&p3.x, &p3.y);

    float triangle[] = {
        p1.x, p1.y, p1.r, p1.g, p1.b,
        p2.x, p2.y, p2.r, p2.g, p2.b,
        p3.x, p3.y, p3.r, p3.g, p3.b
    };

    GLFWwindow* window = initWindow();
    if (!window) {
        std::cout << "There was an error setting up the window" << std::endl;
        return 1;
    }

    GLuint VBO[1], VAO[1];

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint vertexShader = createShader("vert.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader("frag.glsl", GL_FRAGMENT_SHADER);

    GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(*VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
