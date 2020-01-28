#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * BELOW IS A BUNCH OF HELPER CODE
 * You do not need to understand what is going on with it, but if you want to
 * know, let me know and I can walk you through it.
 */

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

std::string readFile(const std::string& fileName) {
    std::ifstream stream(fileName);
    std::stringstream buffer;
    buffer << stream.rdbuf();

    std::string source = buffer.str();
    std::cout << "Source:" << std::endl;
    std::cout << source << std::endl;

    return source;
}

/** END OF CODE THAT YOU DON'T NEED TO WORRY ABOUT */

GLuint createShader(const std::string& fileName, GLenum shaderType) {
    std::string source = readFile(fileName);
    const char* src_ptr = source.c_str();

    /** YOU WILL ADD CODE STARTING HERE */
    GLuint shader = 0;
    // create the shader using
    // glCreateShader, glShaderSource, and glCompileShader
    /** END CODE HERE */

    // Perform some simple error handling on the shader
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
    /** YOU WILL ADD CODE STARTING HERE */
    // create the program using glCreateProgram, glAttachShader, glLinkProgram
    GLuint program = 0;
    /** END CODE HERE */

    // Perform some simple error handling
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
    GLFWwindow* window = initWindow();
    if (!window) {
        std::cout << "There was an error setting up the window" << std::endl;
        return 1;
    }

    /** YOU WILL ADD DATA INITIALIZATION CODE STARTING HERE */

    /* PART1: ask the user for coordinates and colors, and convert to normalized
     * device coordinates */

    // convert the triangle to an array of floats containing
    // normalized device coordinates and color components.
    // float triangle[] = ...

    /** PART2: map the data

    // create vertex and array buffer objects using
    // glGenBuffers, glGenVertexArrays
    GLuint VBO[1], VAO[1];

    // setup triangle using glBindVertexArray, glBindBuffer, GlBufferData

    // setup the attribute pointer for the coordinates
    // setup the attribute pointer for the colors
    // both will use glVertexAttribPointer and glEnableVertexAttribArray;

    /** PART3: create the shader program */

    // create the shaders
    // YOU WILL HAVE TO ADD CODE TO THE createShader FUNCTION ABOVE
    GLuint vertexShader = createShader("../vert.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader("../frag.glsl", GL_FRAGMENT_SHADER);

    // create the shader program
    // YOU WILL HAVE TO ADD CODE TO THE createShaderProgram FUNCTION ABOVE
    GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    // cleanup the vertex and fragment shaders using glDeleteShader

    /** END INITIALIZATION CODE */

    while (!glfwWindowShouldClose(window)) {
        // you don't need to worry about processInput, all it does is listen
        // for the escape character and terminate when escape is pressed.
        processInput(window);

        /** YOU WILL ADD RENDERING CODE STARTING HERE */
        /** PART4: Implemting the rendering loop */

        // clear the screen with your favorite color using glClearColor
        glClear(GL_COLOR_BUFFER_BIT);

        // set the shader program using glUseProgram

        // bind the vertex array using glBindVertexArray

        // draw the triangles using glDrawArrays


        /** END RENDERING CODE */

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
