//
// Created by boris on 8/23/2022.
//

#include "SkeletalMeshRenderData.h"
#include "SkeletalMeshElement.h"
#include "Render/opengl_glad.h"

SkeletalMeshRenderData::SkeletalMeshRenderData()
{
	glGenVertexArrays(1, &VAO);

	bind();
	vertices.attachToAttribute(0);
	textureCoordinate.attachToAttribute(2);
	indices.attachToShader();
	unbind();
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

void SkeletalMeshRenderData::loadMesh(const SkeletalMeshElement& skeletalMesh)
{
		vertices.set(skeletalMesh.vertices);
		normals.set(skeletalMesh.normals);
		textureCoordinate.set(skeletalMesh.textureCoordinates);
		indices.set(skeletalMesh.indices);
		influenceBoneIndices.set(skeletalMesh.influenceBoneIndices);
		influenceBoneWeight.set(skeletalMesh.influenceBoneWeight);
}
