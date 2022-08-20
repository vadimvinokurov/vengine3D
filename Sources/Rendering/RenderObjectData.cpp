//
// Created by boris on 8/20/2022.
//

#include "RenderObjectData.h"

#include <glad/glad.h>

RenderableObjectData::RenderableObjectData(){
	glGenVertexArrays(1, &VAO);


}

RenderableObjectData::~RenderableObjectData()
{
	glDeleteVertexArrays(1, &VAO);
}

void RenderableObjectData::bind()
{
	glBindVertexArray(VAO);
}

void RenderableObjectData::unbind()
{
	glBindVertexArray(0);
}
