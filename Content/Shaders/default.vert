#version 330 core
in vec3 aPosition;
in vec4 aColor;
void main() {
 gl_Position = vec4(aPosition, 1.0);
}