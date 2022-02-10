#version 330 core

uniform vec3 color;
uniform vec3 light;

//in vec3 norm;
//in vec3 fragPos;

void main() {
      vec4 diffuseColor = vec4(color, 1.0f);
//    vec3 n = normalize(norm);
//    vec3 l = normalize(light);
//    float diffuseIntensity = clamp(dot(n, l), 0, 1);

    gl_FragColor = diffuseColor;// * diffuseIntensity;
}