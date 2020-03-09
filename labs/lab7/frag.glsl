#version 330 core

in vec2 uv;

uniform sampler2D aText;

out vec4 fragColor;

void main() {
    fragColor = texture(aText, uv);
}
