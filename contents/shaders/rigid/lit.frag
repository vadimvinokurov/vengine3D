#version 330 core

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;
void main() {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseIntensity = clamp(dot(norm, lightDir) + 0.1, 0, 1);

    vec3 result = objectColor * diffuseIntensity;
    FragColor = vec4(result, 1.0);
}