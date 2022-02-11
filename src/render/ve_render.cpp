//
// Created by boris on 11/16/2021.
//
#include "ve_render.h"
#include "ve_uniform.h"
#include "ve_attribute.h"
#include "ve_index_buffer.h"
#include "ve_draw.h"

//#include "ve_texture.h"


#include <iostream>

using namespace VE;

Render::Render(float windowAspectRatio) : windowAspectRatio_(windowAspectRatio) {
    shader.load("../shaders/static.vert", "../shaders/lit.frag");
    shader.bind();
}

void Render::draw(const WorldPtr &world) {
    world_ = world;
    Matrix4 projection = Camera::perspective(60.0f, windowAspectRatio_, 2, 8000);
    Matrix4 view = world_->currentCamera().getViewMatrix();


    Vector3 lightPoint = globalParameters.ligthPosition;

    if (globalParameters.polygone)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Uniform<Vector3>::set(shader.getUniform("lightPos"), lightPoint);
    Uniform<Matrix4>::set(shader.getUniform("projection"), projection);
    Uniform<Matrix4>::set(shader.getUniform("view"), view);
    for (VE::RigidBodyPtr rigidBody: world_->worldObjects) {
        Matrix4 model = rigidBody->transform().toMatrix();
        Uniform<Matrix4>::set(shader.getUniform("model"), model);
        Uniform<Vector3>::set(shader.getUniform("objectColor"), rigidBody->color());
        Uniform<Vector3>::set(shader.getUniform("lightColor"), Vector3(1, 1, 1));

        for (auto &collider: rigidBody->colliders()) {
            collider->vertexPosition.bindTo(shader.getAttribute("aPosition"));
            collider->vertexNormals.bindTo(shader.getAttribute("aNormal"));

            VE::draw(collider->indexBuffer, DrawMode::Triangles);

            collider->vertexPosition.unBindFrom(shader.getAttribute("aPosition"));
            collider->vertexNormals.unBindFrom(shader.getAttribute("aNormal"));
        }
    }
    Uniform<Matrix4>::set(shader.getUniform("model"), Matrix4());
    Uniform<Vector3>::set(shader.getUniform("objectColor"), Color(0, 0, 0));

    lightPoint.drawPoint(24);
}

