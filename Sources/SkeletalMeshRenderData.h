//
// Created by boris on 8/23/2022.
//

#ifndef VENGINE3D_SKELETALMESHRENDERDATA_H
#define VENGINE3D_SKELETALMESHRENDERDATA_H

#include "Math/Vector.h"
#include "Render/ObjectBuffer.h"
#include "Render/IndexBuffer.h"

struct SkeletalMeshRenderData
{
	SkeletalMeshRenderData();
	~SkeletalMeshRenderData();

	SkeletalMeshRenderData(const SkeletalMeshRenderData& other) = delete;
	SkeletalMeshRenderData& operator=(const SkeletalMeshRenderData& other) = delete;
	SkeletalMeshRenderData(SkeletalMeshRenderData&& other) = delete;
	SkeletalMeshRenderData& operator=(SkeletalMeshRenderData&& other) = delete;

	void bind();
	void unbind();

	Render::ObjectBuffer<Vector3> vertices;
	Render::ObjectBuffer<Vector3> normals;
	Render::ObjectBuffer<Vector2> textureCoordinate;
	Render::IndexBuffer indices;
	Render::ObjectBuffer<IVector4> influenceBoneIndices;
	Render::ObjectBuffer<Vector4> influenceBoneWeight;
	uint32 VAO;
};

#endif // VENGINE3D_SKELETALMESHRENDERDATA_H
