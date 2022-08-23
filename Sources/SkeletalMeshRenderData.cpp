//
// Created by boris on 8/23/2022.
//

#include "SkeletalMeshRenderData.h"
#include "Render/opengl_glad.h"

SkeletalMeshRenderData::SkeletalMeshRenderData()
{
	glGenVertexArrays(1, &VAO);
}
SkeletalMeshRenderData::~SkeletalMeshRenderData()
{
	glDeleteVertexArrays(1, &VAO);
}
void SkeletalMeshRenderData::bind()
{
	glBindVertexArray(VAO);
}
void SkeletalMeshRenderData::unbind()
{
	glBindVertexArray(0);
}
