//
// Created by boris on 2/6/2022.
//
#include "ve_shader.h"
#include <glad/glad.h>

VE::Shader::Shader() {
    handle_ = glCreateProgram();
}

VE::Shader::Shader(const std::string &vertex, const std::string &fragment) {
    handle_ = glCreateProgram();
    Load(vertex, fragment);
}

VE::Shader::~Shader() {
    glDeleteProgram(handle_);
}

void VE::Shader::Load(const std::string &vertex, const std::string &fragment) {
    std::string vertexSource = readFile(vertex);
    std::string fragmentSource = readFile(fragment);

    unsigned int v = compileVertexShade(vertexSource);
    unsigned int f = compileVertexShade(fragmentSource);
    if (linkShaders(v, f)) {
        populateAttributes();
        populateUniforms();
    }
}

void VE::Shader::Bind() {
    glUseProgram(handle_);
}

void VE::Shader::UnBind() {
    glUseProgram(0);
}

unsigned int VE::Shader::getAttribute(const std::string &name) {
    auto it = attributes_.find(name);
    if (it == attributes_.end()) {
        std::cout << "Bad attribute index: " << name << std::endl;
        return 0;
    }
    return it->second;
}

unsigned int VE::Shader::getUniform(const std::string &name) {
    auto it = uniforms_.find(name);
    if (it == uniforms_.end()) {
        std::cout << "Bad uniforms index: " << name << std::endl;
        return 0;
    }
    return it->second;
}

unsigned int VE::Shader::getHandle() {
    return handle_;
}

std::string VE::Shader::readFile(const std::string &path) {
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) return path;
    std::stringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

unsigned int VE::Shader::compileVertexShade(const std::string &vertex) {
    unsigned int v = glCreateShader(GL_VERTEX_SHADER);
    const char *v_source = vertex.c_str();

    glShaderSource(v, 1, &v_source, NULL);
    glCompileShader(v);
    int success = 0;
    glGetShaderiv(v, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(v, 512, NULL, infoLog);
        std::string info = "Vertex compilation failed. " + std::string(infoLog);
        std::cout << info << std::endl;
        glDeleteShader(v);

        return 0;
    }
    return v;
}

unsigned int VE::Shader::compileFragmentShader(const std::string &fragment) {
    unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
    const char *f_source = fragment.c_str();

    glShaderSource(f, 1, &f_source, NULL);
    glCompileShader(f);
    int success = 0;
    glGetShaderiv(f, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(f, 512, NULL, infoLog);
        std::string info = "Fragment compilation failed. " + std::string(infoLog);
        std::cout << info << std::endl;

        glDeleteShader(f);
        return 0;
    }
    return f;
}

bool VE::Shader::linkShaders(unsigned int vertex, unsigned int fragment) {
    glAttachShader(handle_, vertex);
    glAttachShader(handle_, fragment);
    glLinkProgram(handle_);

    int success = 0;
    glGetProgramiv(handle_, GL_LINK_STATUS, &success);
    if (success) {
        char infoLog[512];
        glGetProgramInfoLog(handle_, 512, NULL, infoLog);
        std::string info = "ERROR: Shader linking failed. " + std::string(infoLog);
        std::cout << info << std::endl;

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return false;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

void VE::Shader::populateAttributes() {
    int count = -1;
    int length;
    char name[128];
    int size;
    GLenum type;

    glUseProgram(handle_);
    glGetProgramiv(handle_, GL_ACTIVE_ATTRIBUTES, &count);

    for (int i = 0; i < count; ++i) {
        memset(name, 0, sizeof(char) * 128);
        glGetActiveAttrib(handle_, (GLuint) i, 128, &length, &size, &type, name);
        int attrib = glGetAttribLocation(handle_, name);
        if (attrib >= 0) {
            attributes_[name] = attrib;
        }
    }
    glUseProgram(0);
}

void VE::Shader::populateUniforms() {
    int count = -1;
    int length;
    char name[128];
    int size;
    GLenum type;
    char testName[256];

    glUseProgram(handle_);
    glGetProgramiv(handle_, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; ++i) {
        memset(name, 0, sizeof(char) * 128);
        glGetActiveUniform(handle_, (GLuint) i, 128, &length, &size, &type, name);
        int unifrom = glGetUniformLocation(handle_, name);
        if (unifrom >= 0) {
            std::string uniformName = name;
            auto found = uniformName.find('[');
            if (found != std::string::npos) {
                uniformName.erase(uniformName.begin() + found, uniformName.end());

                unsigned int uniformIndex = 0;
                while (true) {
                    memset(testName, 0, sizeof(char) * 256);
                    sprintf(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);
                    int uniformLocation = glGetUniformLocation(handle_, testName);
                    if (uniformLocation < 0) {
                        break;
                    }
                    uniforms_[testName] = uniformLocation;
                }
            }
            uniforms_[uniformName] = unifrom;
        }
    }
    glUseProgram(0);
}