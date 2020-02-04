#version 330 core

out vec4 fragColor;
in vec3 pColor;

void main() {
    fragColor = vec4(pColor, 1.0f);
}
