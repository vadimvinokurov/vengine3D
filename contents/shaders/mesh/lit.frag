#version 330 core

uniform vec3 lightPos;
uniform sampler2D tex0;

in vec3 normal;
in vec3 fragPos;
in vec2 uv;

out vec4 FragColor;
void main() {
    vec4 diffuseColor = texture(tex0, uv);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseIntensity = clamp(dot(norm, lightDir) + 0.1, 0, 1);

    FragColor = diffuseColor * diffuseIntensity;
}