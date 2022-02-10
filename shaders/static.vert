#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;
//in vec3 normal;
//
//out vec3 norm;
//out vec3 fragPos;

void main() {
 gl_Position = projection * view * model * vec4(position, 1.0);
// fragPos = vec3(model * vec4(position, 1.0));
// norm = vec3(model * vec4(normal, 0.0));

}