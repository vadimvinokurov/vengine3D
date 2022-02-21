#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoor;

out vec3 normal;
out vec3 fragPos;
out vec2 uv;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    fragPos = vec3(model * vec4(aPosition, 1.0));
    normal = vec3(model * vec4(aNormal, 0.0));
    uv = aTexCoor;
}