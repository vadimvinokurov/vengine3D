//
// Created by boris on 8/31/2022.
//

#include "VAO.h"
#include "Render/opengl_glad.h"

VAO::VAO()
{
	glGenVertexArrays(1, &handle_);
}
VAO::~VAO()
{
	if (handle_ == 0)
	{
		return;
	}
	glDeleteVertexArrays(1, &handle_);
}
void VAO::bind() const
{
	glBindVertexArray(handle_);
}
void VAO::unbind() const
{
	glBindVertexArray(0);
}
VAO::VAO(VAO &&other)
{
	handle_ = other.handle_;
	other.handle_ = 0;
}
VAO &VAO::operator=(VAO &&other)
{
	handle_ = other.handle_;
	other.handle_ = 0;
	return *this;
}
