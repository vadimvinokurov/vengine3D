#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 aTextCoord;
layout(location = 3) in ivec4 aBoneIds;
layout(location = 4) in vec4 aWeight;

out vec2 fTextCoord;
void main() {
    mat4 b =    bones[aBoneIds[0]] * aWeight[0] +
                bones[aBoneIds[1]] * aWeight[1] +
                bones[aBoneIds[2]] * aWeight[2] +
                bones[aBoneIds[3]] * aWeight[3];

 gl_Position = projection * view * model * transpose(b) * vec4(aPosition, 1.0);
 fTextCoord = aTextCoord;
}