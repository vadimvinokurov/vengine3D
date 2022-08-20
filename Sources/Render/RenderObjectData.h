//
// Created by boris on 8/20/2022.
//

#ifndef VENGINE3D_RENDEROBJECTDATA_H
#define VENGINE3D_RENDEROBJECTDATA_H

#include "IndexBuffer.h"
#include "ObjectBuffer.h"
#include "Math/Vector.h"
#include "Render/Shader.h"

struct RenderableObjectData
{
	RenderableObjectData();
	~RenderableObjectData();

	void bind();
	void unbind();

	Render::ObjectBuffer<Vector3> verticesBuffer;
	Render::ObjectBuffer<Vector3> normalsBuffer;
	Render::ObjectBuffer<Vector2> textureCoordinatesBuffer;
	Render::IndexBuffer indicesBuffer;

	Shader shader;

	uint32 VAO;
};
#endif // VENGINE3D_RENDEROBJECTDATA_H
