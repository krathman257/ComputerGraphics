#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;

uniform mat3 normTransform;

out vec3 ourPos;
out vec3 ourNorm;
out vec3 ourColor;

void main() {
    vec4 temp = projection * camera * model * vec4(aPos, 1.0);

    ourPos = vec3(temp.xyz);
    ourNorm = normalize(normTransform * aNorm);
    ourColor = aColor;

    gl_Position = temp;
}
