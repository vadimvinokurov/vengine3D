#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 aTextCoord;

out vec2 fTextCoord;
void main() {
 gl_Position = projection * view * model * vec4(aPosition, 1.0);
 fTextCoord = aTextCoord;
}