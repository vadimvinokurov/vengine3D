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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClearDepth(1.0f);
}

void Render::draw(const WorldPtr &world) {
    world_ = world;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    Matrix4 projection = Camera::perspective(60.0f, windowAspectRatio_, 2, 8000);
    Matrix4 view = world_->currentCamera().getViewMatrix();



    Uniform<Vector3>::set(shader.getUniform("light"), Vector3(0, 10, 10));

    Uniform<Matrix4>::set(shader.getUniform("projection"), projection);
    Uniform<Matrix4>::set(shader.getUniform("view"), view);
    for (VE::RigidBodyPtr rigidBody: world_->worldObjects) {
        Matrix4 model = rigidBody->transform().toMatrix();
        Uniform<Matrix4>::set(shader.getUniform("model"), model);
        Uniform<Vector3>::set(shader.getUniform("color"), rigidBody->color());

        for (size_t colliderNumber = 0; colliderNumber < rigidBody->colliders().size(); colliderNumber++) {
            const auto& collider = *(rigidBody->colliders()[colliderNumber]);
            Attribute<Vector3> vertexPosition;
            vertexPosition.set(collider.vertices());

            Attribute<Vector3> vertexNormals;
            vertexNormals.set(collider.normals());

            IndexBuffer indexBuffer;
            indexBuffer.set(collider.indices());

            vertexPosition.bindTo(shader.getAttribute("position"));
            vertexNormals.bindTo(shader.getAttribute("normal"));
            VE::draw(indexBuffer, DrawMode::Triangles);

            vertexPosition.unBindFrom(shader.getAttribute("position"));
            vertexNormals.unBindFrom(shader.getAttribute("normal"));
        }
    }
    Uniform<Matrix4>::set(shader.getUniform("model"), Matrix4());
    shader.bind();
}

