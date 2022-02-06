//
// Created by boris on 2/6/2022.
//
#include "ve_shader.h"
#include <glad/glad.h>

VE::Shader::Shader() {
    glCreateProgram();
}

VE::Shader::~Shader() {

}


VE::Shader::Shader(const std::string &vertex, const std::string &fragment) {

}

void VE::Shader::Load(const std::string &vertex, const std::string &fragment) {

}

void VE::Shader::Bind() {

}

void VE::Shader::UnBind() {

}

unsigned int VE::Shader::getAttribute(const std::string &name) {
    return 0;
}

unsigned int VE::Shader::getUniform(const std::string &name) {
    return 0;
}

unsigned int VE::Shader::getHandle() {
    return 0;
}

std::string VE::Shader::readFile(const std::string &path) {
    return std::__cxx11::string();
}

unsigned int VE::Shader::compileVertexShade(const std::string &vertex) {
    return 0;
}

unsigned int VE::Shader::compileFragmentShader(const std::string &fragment) {
    return 0;
}

bool VE::Shader::linkShaders(unsigned int vertex, unsigned int fragment) {
    return false;
}

void VE::Shader::populateAttributes() {

}

void VE::Shader::populateUniforms() {

}