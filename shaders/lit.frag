#version 330 core

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;
void main() {
    // Окружающая составляющая
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Диффузная составляющая
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos) * -1;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}