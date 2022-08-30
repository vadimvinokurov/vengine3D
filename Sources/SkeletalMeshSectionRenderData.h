//
// Created by boris on 8/23/2022.
//

#ifndef VENGINE3D_SKELETALMESHSECTIONRENDERDATA_H
#define VENGINE3D_SKELETALMESHSECTIONRENDERDATA_H

#include "Math/Vector.h"
#include "Render/ObjectBuffer.h"
#include "Render/IndexBuffer.h"

class SkeletalMeshSection;

struct SkeletalMeshSectionRenderData
{
	explicit SkeletalMeshSectionRenderData(const SkeletalMeshSection &skeletalMesh)
	{
		vertices.set(skeletalMesh.vertices);
		normals.set(skeletalMesh.normals);
		textureCoordinate.set(skeletalMesh.textureCoordinates);
		indices.set(skeletalMesh.indices);
		influenceBoneIndices.set(skeletalMesh.influenceBoneIndices);
		influenceBoneWeight.set(skeletalMesh.influenceBoneWeight);
	}

	void use()
	{
		vertices.attachToAttribute(0);
		textureCoordinate.attachToAttribute(2);
		indices.attachToShader();
	}

	Render::ObjectBuffer<Vector3> vertices;
	Render::ObjectBuffer<Vector3> normals;
	Render::ObjectBuffer<Vector2> textureCoordinate;
	Render::IndexBuffer indices;
	Render::ObjectBuffer<IVector4> influenceBoneIndices;
	Render::ObjectBuffer<Vector4> influenceBoneWeight;
};

#endif // VENGINE3D_SKELETALMESHSECTIONRENDERDATA_H
