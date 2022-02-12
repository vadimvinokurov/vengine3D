#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 aPosition;
void main() {
 gl_Position = projection * view * model * vec4(aPosition, 1.0);
}