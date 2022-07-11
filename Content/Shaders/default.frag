#version 330 core

in vec2 fTextCoord;
out vec4 FragColor;

uniform sampler2D tex0;
void main() {
    FragColor = texture(tex0,fTextCoord);
}