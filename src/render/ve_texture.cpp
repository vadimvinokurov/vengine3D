//
// Created by boris on 2/7/2022.
//
#include "glad/glad.h"
#include "ve_texture.h"
#include "stb_image.h"

VE::Texture::Texture() : width_(0),
                         height_(0),
                         channels_(0) {

    glGenTextures(1, &handle_);
}

VE::Texture::Texture(const char *path) : Texture() {
    load(path);
}

VE::Texture::~Texture() {
    glDeleteTextures(1, &handle_);
}

void VE::Texture::load(const char *path) {
    glBindTexture(GL_TEXTURE_2D, handle_);
    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    width_ = width;
    height_ = height;
    channels_ = channels;
}

void VE::Texture::set(unsigned int uniformIndex, unsigned int textureIndex) {
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, handle_);
    glUniform1i(uniformIndex, textureIndex);
}

void VE::Texture::unSet(unsigned int textureIndex) {
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

unsigned int VE::Texture::getHandle() {
    return handle_;
}
