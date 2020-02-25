#version 330 core
in vec3 ourPos;
in vec3 ourNorm;
in vec3 ourColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;

uniform vec3 lightPos;
uniform float specular;
uniform float ambient;

uniform vec3 lightColor;
uniform float lightIntensity;

out vec4 fragColor;

void main() {
    //Set initial vectors
    vec4 ourColor4 = vec4(ourColor, 1.0f);
    vec4 lightColor4 = vec4(lightColor, 1.0f);
    vec4 lightPos4 = projection * camera * vec4(lightPos, 1.0f);
    vec3 lightPosTrans = vec3(lightPos4.xyz);

    //Diffuse
    vec3 toLightSource = lightPosTrans - ourPos;
    //vec3 toLightSource = ourPos - lightPosTrans;
    float dotProd = clamp(dot(toLightSource, ourNorm), 0, 1);

    //Specular
    vec3 e = normalize(vec3(ourPos.xyz) * -1);//, ourPos.z-2));
    vec3 r = normalize(reflect(toLightSource, ourNorm));

    float spec = pow(max(0, dot(e, r)), specular);
    vec4 specVec = vec4(1.0f, 1.0f, 1.0f, 1.0f) * spec;

    //Ambient
    vec4 ambiVec = ourColor4 * ambient;

    fragColor = dotProd * ourColor4 + specVec + ambiVec;
}
