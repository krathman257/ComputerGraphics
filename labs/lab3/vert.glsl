#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 myColor;

uniform mat3 transPos;

void main() {
    vec3 temp = vec3(aPos, 1.0);
    temp = transPos * temp;
    gl_Position = vec4(temp.x, temp.y, 1.0, 1.0);
    myColor = aColor;
}
