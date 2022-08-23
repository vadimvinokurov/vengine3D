//
// Created by boris on 2/7/2022.
//
#include "IndexBuffer.h"

#include "Render/opengl_glad.h"

using namespace Render;

IndexBuffer::IndexBuffer(IndexBuffer &&other)
{
	this->count_ = other.count_;
	this->handle_ = other.handle_;

	other.count_ = 0;
	other.handle_ = 0;
}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other)
{
	this->count_ = other.count_;
	this->handle_ = other.handle_;

	other.count_ = 0;
	other.handle_ = 0;

	return *this;
}

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &handle_);
}

IndexBuffer::~IndexBuffer()
{
	count_ = 0;
	if (handle_ == 0)
	{
		return;
	}
	glDeleteBuffers(1, &handle_);
}

void IndexBuffer::set(const uint32 *inputArray, uint32 arrayLength)
{
	count_ = arrayLength;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * count_, inputArray, GL_STATIC_DRAW);
}

void IndexBuffer::set(const std::vector<unsigned int> &input)
{
	set(input.data(), input.size());
}

void IndexBuffer::attachToShader()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
}

uint32 IndexBuffer::count()
{
	return count_;
}

uint32 IndexBuffer::getHandle()
{
	return handle_;
}
