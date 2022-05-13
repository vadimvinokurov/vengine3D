//
// Created by boris on 11/16/2021.
//
#include "ve_render.h"
#include <glad/glad.h>

#include "ve_uniform.h"
#include "ve_attribute.h"
#include "ve_index_buffer.h"
#include "ve_draw.h"
#include "ve_debug_draw.h"

//#include "ve_texture.h"
#include <iostream>

using namespace VE;

Render::Render(float windowAspectRatio) : windowAspectRatio_(windowAspectRatio) {
	texture_.load("../contents/assets/stormtrooper/textures/Stormtroopermat_baseColor.png");
	//texture_.load("../contents/assets/woman/Woman.png");
	rigidShader.load("../contents/shaders/rigid/static.vert", "../contents/shaders/rigid/lit.frag");
	meshShader.load("../contents/shaders/mesh/static.vert", "../contents/shaders/mesh/lit.frag");
	debugShader.load("../contents/shaders/debug/static.vert", "../contents/shaders/debug/lit.frag");
	debugShader.bind();
	DebugDraw::colorShaderSlot = debugShader.getUniform("objectColor");
}

void Render::draw(const WorldPtr& world) {
	debugShader.unBind();
	rigidShader.bind();
	world_ = world;
	Matrix4 projection = Camera::perspective(60.0f, windowAspectRatio_, 2, 8000);
	Matrix4 view = world_->currentCamera().getViewMatrix();


	Vector3 lightPoint = globalParameters.ligthPosition;

	if (globalParameters.polygone) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Uniform<Vector3>::set(rigidShader.getUniform("lightPos"), lightPoint);
	Uniform<Matrix4>::set(rigidShader.getUniform("projection"), projection);
	Uniform<Matrix4>::set(rigidShader.getUniform("view"), view);
	for (VE::RigidBodyPtr rigidBody : world_->worldObjects) {
		Matrix4 model = rigidBody->transform().toMatrix();
		Uniform<Matrix4>::set(rigidShader.getUniform("model"), model);
		Uniform<Vector3>::set(rigidShader.getUniform("objectColor"), Vector3(rigidBody->color().v));

		for (auto& collider : rigidBody->colliders()) {
			collider->vertexPosition.bindTo(rigidShader.getAttribute("aPosition"));
			collider->vertexNormals.bindTo(rigidShader.getAttribute("aNormal"));

			VE::draw(collider->indexBuffer, DrawMode::Triangles);

			collider->vertexPosition.unBindFrom(rigidShader.getAttribute("aPosition"));
			collider->vertexNormals.unBindFrom(rigidShader.getAttribute("aNormal"));
		}
	}
	rigidShader.unBind();


	meshShader.bind();
	texture_.set(meshShader.getUniform("tex0"), 0);
	Uniform<Vector3>::set(meshShader.getUniform("lightPos"), lightPoint);
	Uniform<Matrix4>::set(meshShader.getUniform("projection"), projection);
	Uniform<Matrix4>::set(meshShader.getUniform("view"), view);


	Uniform<Matrix4>::set(meshShader.getUniform("model"), (Transform()).toMatrix());

	for (auto&& mesh : world_->meshes) {
		mesh.positionsGPU.bindTo(meshShader.getAttribute("aPosition"));
		mesh.normalsGPU.bindTo(meshShader.getAttribute("aNormal"));
		mesh.textureCoordinatesGPU.bindTo(meshShader.getAttribute("aTexCoor"));

		VE::draw(mesh.indicesGPU, DrawMode::Triangles);

		mesh.textureCoordinatesGPU.unBindFrom(meshShader.getAttribute("aTexCoor"));
		mesh.positionsGPU.unBindFrom(meshShader.getAttribute("aPosition"));
		mesh.normalsGPU.unBindFrom(meshShader.getAttribute("aNormal"));
	}
	texture_.unSet(0);
	meshShader.unBind();


	debugShader.bind();
	Uniform<Matrix4>::set(debugShader.getUniform("projection"), projection);
	Uniform<Matrix4>::set(debugShader.getUniform("view"), view);
	Uniform<Matrix4>::set(debugShader.getUniform("model"), Matrix4());
}
