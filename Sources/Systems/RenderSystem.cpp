//
// Created by boris on 8/15/2022.
//

#include "RenderSystem.h"
#include "World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponents.h"
#include "Components/CameraComponent.h"
#include "Render/Uniform.h"
#include "Render/opengl_glad.h"

RenderSystem::RenderSystem(SystemPriority priority)
	: System(priority), defaultVertexShader("../Content/Shaders/default.vert", ShaderType::Vertex)
{
	gladLoadGL();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);
}

void RenderSystem::onWindowResize(int32 width, int32 height)
{
	glViewport(0, 0, width, height);
	float windowAspectRatio = static_cast<float>(width) / static_cast<float>(height);
	perspective = CameraComponent::perspective(60.0f, windowAspectRatio, 0.1f, 8000);
}

void RenderSystem::preUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RenderSystem::update(float dt)
{
	float aspectRatio = 0;
	CameraComponent *cameraComponent = getMainCamera();
	if (cameraComponent != nullptr)
	{
		view = cameraComponent->getViewMatrix();
	}

	auto [staticMeshComp_begin, staticMeshComp_end] = getWorld()->getComponents<StaticMeshComponent>();
	for (auto it = staticMeshComp_begin; it != staticMeshComp_end; ++it)
	{
		updateStaticMeshComponent(&(*it), dt);
	}
}

void RenderSystem::postUpdate(float dt)
{
}
void RenderSystem::updateStaticMeshComponent(StaticMeshComponent *staticMeshComponent, float dt)
{
	auto &shader = staticMeshComponent->renderData.shader;
	auto &renderData = staticMeshComponent->renderData;
	auto &staticMesh = *staticMeshComponent->staticMesh;
	auto &material = *staticMeshComponent->material;

	if (staticMeshComponent->needUpdateRenderData)
	{

		std::vector<ShaderSource> shaderSource = {defaultVertexShader};
		shaderSource.insert(shaderSource.end(), material.vertexShader.begin(), material.vertexShader.end());
		shader = std::make_unique<Shader>(shaderSource);
		staticMeshComponent->needUpdateMaterialData = 0;
		spdlog::info("Update shaders data");
	}

	shader->bind();
	if (staticMeshComponent->needUpdateRenderData)
	{
		renderData.bind();

		renderData.verticesBuffer.set(staticMesh.vertices);
		renderData.normalsBuffer.set(staticMesh.normals);
		renderData.textureCoordinatesBuffer.set(staticMesh.textureCoordinates);
		renderData.indicesBuffer.set(staticMesh.indices);

		renderData.verticesBuffer.attachToAttribute(shader->getAttribute("aPosition"));
		// renderData.normalsBuffer.attachToAttribute(shader->getAttribute("aNormal"));
		renderData.textureCoordinatesBuffer.attachToAttribute(shader->getAttribute("aTextCoord"));
		renderData.indicesBuffer.attachToShader();

		renderData.unbind();
		staticMeshComponent->needUpdateRenderData = 0;
		spdlog::info("Update render data");
	}

	for (auto &texture : material.textures)
	{
		texture.first.bind(shader->getUniform(texture.second));
	}

	Render::Uniform<Matrix4>::set(shader->getUniform("projection"), perspective);
	Render::Uniform<Matrix4>::set(shader->getUniform("view"), view);
	Render::Uniform<Matrix4>::set(shader->getUniform("model"), Matrix4());

	renderData.bind();
	glDrawElements(GL_TRIANGLES, renderData.indicesBuffer.count(), GL_UNSIGNED_INT, 0);
	renderData.unbind();

	shader->unBind();
}

CameraComponent *RenderSystem::getMainCamera()
{
	auto [inputComponentIt, end] = getWorld()->getComponents<InputComponents>();
	if (inputComponentIt == end)
	{
		return nullptr;
	}
	auto entity = getWorld()->getEntityByEntityId(inputComponentIt->getOwner());
	if (entity == nullptr)
	{
		return nullptr;
	}
	return entity->getComponent<CameraComponent>();
}
