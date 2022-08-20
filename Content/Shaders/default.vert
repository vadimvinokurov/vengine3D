#version 330 core
in vec3 aPosition;
in vec3 aNormals;
in vec2 aTextCoord;
out vec2 fTextCoord;
void main() {
 gl_Position = vec4(aPosition, 1.0);
 fTextCoord = aTextCoord;
}