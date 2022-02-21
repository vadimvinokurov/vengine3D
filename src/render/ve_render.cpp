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
	shader.load("../contents/shaders/static.vert", "../contents/shaders/lit.frag");
	debugShader.load("../contents/shaders/debug/static.vert", "../contents/shaders/debug/lit.frag");
	debugShader.bind();
	DebugDraw::colorShaderSlot = debugShader.getUniform("objectColor");
}

void Render::draw(const WorldPtr& world) {
	debugShader.unBind();
	shader.bind();
	world_ = world;
	Matrix4 projection = Camera::perspective(60.0f, windowAspectRatio_, 2, 8000);
	Matrix4 view = world_->currentCamera().getViewMatrix();


	Vector3 lightPoint = globalParameters.ligthPosition;

	if (globalParameters.polygone) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Uniform<Vector3>::set(shader.getUniform("lightPos"), lightPoint);
	Uniform<Matrix4>::set(shader.getUniform("projection"), projection);
	Uniform<Matrix4>::set(shader.getUniform("view"), view);

	for (VE::RigidBodyPtr rigidBody : world_->worldObjects) {
		Matrix4 model = rigidBody->transform().toMatrix();
		Uniform<Matrix4>::set(shader.getUniform("model"), model);
		Uniform<Vector3>::set(shader.getUniform("objectColor"), Vector3(rigidBody->color().v));
		//Uniform<Vector3>::set(shader.getUniform("lightColor"), Vector3(1, 1, 1));

		for (auto& collider : rigidBody->colliders()) {
			collider->vertexPosition.bindTo(shader.getAttribute("aPosition"));
			collider->vertexNormals.bindTo(shader.getAttribute("aNormal"));

			VE::draw(collider->indexBuffer, DrawMode::Triangles);

			collider->vertexPosition.unBindFrom(shader.getAttribute("aPosition"));
			collider->vertexNormals.unBindFrom(shader.getAttribute("aNormal"));
		}
	}

	Transform t1(Quaternion::fromAxisAngle(Vector3(1, 0, 0), M_PI / 2));
	Transform t2(Quaternion::fromAxisAngle(Vector3(0, 0, 1), M_PI / 2));
	Transform t3(Vector3(-5, 0, 0));
	Uniform<Matrix4>::set(shader.getUniform("model"), (t3 * t2 * t1).toMatrix());

	for (auto&& mesh : world_->meshes) {
		mesh.positionsGPU.bindTo(shader.getAttribute("aPosition"));
		mesh.normalsGPU.bindTo(shader.getAttribute("aNormal"));

		VE::draw(mesh.indicesGPU, DrawMode::Triangles);
		//VE::draw(mesh.positions.size(), DrawMode::Triangles);

		mesh.positionsGPU.unBindFrom(shader.getAttribute("aPosition"));
		mesh.normalsGPU.unBindFrom(shader.getAttribute("aNormal"));
	}

	shader.unBind();
	debugShader.bind();
	Uniform<Matrix4>::set(debugShader.getUniform("projection"), projection);
	Uniform<Matrix4>::set(debugShader.getUniform("view"), view);
	Uniform<Matrix4>::set(debugShader.getUniform("model"), Matrix4());
}
