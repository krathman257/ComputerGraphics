#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 scalMat, camMat, persMat, orthoMat;

void main() {
    vec4 temp = vec4(aPos, 1.0);
    gl_Position = orthoMat * persMat * camMat * scalMat * temp;
    ourColor = aColor;
}
