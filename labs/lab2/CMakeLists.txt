cmake_minimum_required(VERSION 3.0)
project(lab2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEPS_DIR "../deps")

set(GLFW_DIR ${DEPS_DIR}/glfw)
add_subdirectory(${GLFW_DIR} ${CMAKE_CURRENT_BINARY_DIR}/glfw)
include_directories(${GLFW_DIR}/include)

set(GLAD_DIR ${DEPS_DIR}/glad)
include_directories(${GLAD_DIR}/include)

set(LAB ${PROJECT_NAME})
add_executable(${LAB} main.cpp ${GLAD_DIR}/src/glad.c)
target_link_libraries(${LAB} glfw ${GLFW_LIBRARIES})

