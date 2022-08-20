//
// Created by boris on 2/7/2022.
//
#include "Texture.h"
#include "stb_image.h"

#include "Render/opengl_glad.h"

Texture::Texture() : width_(0), height_(0), channels_(0)
{
	glGenTextures(1, &handle_);
}

Texture::Texture(const char *path) : Texture()
{
	load(path);
}

Texture::~Texture()
{
	if (handle_ != 0)
	{
		glDeleteTextures(1, &handle_);
	}
}

Texture::Texture(Texture &&other)
{
	this->width_ = other.width_;
	this->height_ = other.height_;
	this->channels_ = other.channels_;
	this->handle_ = other.handle_;

	other.handle_ = 0;
}

Texture &Texture::operator=(Texture &&other)
{
	this->width_ = other.width_;
	this->height_ = other.height_;
	this->channels_ = other.channels_;
	this->handle_ = other.handle_;

	other.handle_ = 0;
	return *this;
}

void Texture::load(const char *path)
{
	int32 width, height, channels;
	unsigned char *data = stbi_load(path, &width, &height, &channels, 4);
	if (!data)
	{
		spdlog::critical("Can't load texture {:s}!", path);
		std::exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, handle_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	width_ = width;
	height_ = height;
	channels_ = channels;
}

void Texture::bind(uint32 uniformIndex, uint32 textureIndex)
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, handle_);
	glUniform1i(uniformIndex, textureIndex);
}

void Texture::unBind(uint32 textureIndex)
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

uint32 Texture::getHandle()
{
	return handle_;
}
