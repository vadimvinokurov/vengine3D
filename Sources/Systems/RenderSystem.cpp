//
// Created by boris on 8/15/2022.
//

#include "RenderSystem.h"
#include "World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

	auto [staticMeshComp_begin, staticMeshComp_end] = VEngine::getWorld()->getComponents<StaticMeshComponent>();
	for (auto it = staticMeshComp_begin; it != staticMeshComp_end; ++it)
	{
		updateStaticMeshComponent(&(*it), dt);
	}

	auto [skeletalMeshComp_begin, skeletalMeshComp_end] = VEngine::getWorld()->getComponents<SkeletalMeshComponent>();
	for (auto it = skeletalMeshComp_begin; it != skeletalMeshComp_end; ++it)
	{
		updateAnimation(&(*it), dt);
		updateSceletalMeshComponent(&(*it), dt);
	}
}

void RenderSystem::postUpdate(float dt)
{
}
void RenderSystem::updateStaticMeshComponent(StaticMeshComponent *staticMeshComponent, float dt)
{
	auto &shader = staticMeshComponent->material->shader;
	auto &renderData = staticMeshComponent->renderData;
	auto &staticMesh = *staticMeshComponent->staticMesh;
	auto &material = *staticMeshComponent->material;

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

	Matrix4 model = staticMeshComponent->transform.toMatrix();

	Render::Uniform<Matrix4>::set(shader->getUniform("projection"), perspective);
	Render::Uniform<Matrix4>::set(shader->getUniform("view"), view);
	Render::Uniform<Matrix4>::set(shader->getUniform("model"), model);

	renderData.bind();
	glDrawElements(GL_TRIANGLES, renderData.indicesBuffer.count(), GL_UNSIGNED_INT, 0);
	renderData.unbind();

	shader->unBind();
}

void RenderSystem::updateAnimation(SkeletalMeshComponent *scm, float dt)
{
	//dt = 0.9f;
	scm->animTime = scm->animation->sample(*scm->skeleton, scm->animTime + dt);
	//exit(0);
}

void RenderSystem::updateSceletalMeshComponent(SkeletalMeshComponent *skeletalMeshComponent, float dt)
{
	auto &skeletalMeshModel_ = skeletalMeshComponent->skeletalMesh.skeletalMeshModel_;
	auto &materials = skeletalMeshComponent->skeletalMesh.materials;

	for (int i = 0; i < skeletalMeshModel_->getSectionCount(); ++i)
	{
		auto &shader = materials[i]->shader;
		shader->bind();

		for (auto &texture : materials[i]->textures)
		{
			texture.first.bind(shader->getUniform(texture.second));
		}
		Matrix4 model = skeletalMeshComponent->transform.toMatrix();
		auto bones = skeletalMeshComponent->skeleton->getMatrixPalette();

		Render::Uniform<Matrix4>::set(shader->getUniform("projection"), perspective);
		Render::Uniform<Matrix4>::set(shader->getUniform("view"), view);
		Render::Uniform<Matrix4>::set(shader->getUniform("model"), model);
		Render::Uniform<Matrix4>::set(shader->getUniform("bones"), bones);

		auto [VAO, indicesCount] = skeletalMeshModel_->getRenderData(i);

		VAO.bind();
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		VAO.unbind();

		shader->unBind();
	}
}

CameraComponent *RenderSystem::getMainCamera()
{
	auto [inputComponentIt, end] = VEngine::getWorld()->getComponents<InputComponents>();
	if (inputComponentIt == end)
	{
		return nullptr;
	}
	auto entity = VEngine::getWorld()->getEntityByEntityId(inputComponentIt->getOwner());
	if (entity == nullptr)
	{
		return nullptr;
	}
	return entity->getComponent<CameraComponent>();
}
