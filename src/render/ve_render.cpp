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

//
//void drawShape(const VE::Collider &shape, const Color &color) {
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 0, shape.verticesGLFormatData());
//    for (int i = 0; i < shape.indecesSize() / 4; i++) {
//        glColor3f(color.red() + i / 100.0, color.grean() + i / 100.0, color.blue() + i / 100.0);
//        glDrawElements(globalParameters.polygone ? GL_POLYGON : GL_LINE_LOOP, 4, GL_UNSIGNED_INT, shape.indicesGLFormatData(i * 4));
//    }
//    glDisableClientState(GL_VERTEX_ARRAY);
//};



Render::Render(float windowAspectRatio) : windowAspectRatio_(windowAspectRatio) {
    shader.load("../shaders/static.vert", "../shaders/lit.frag");
    shader.bind();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClearDepth(1.0f);
}

void Render::draw(const WorldPtr &world) {
    world_ = world;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix4 projection = Camera::perspective(60.0f, windowAspectRatio_, 2, 8000);
    Matrix4 view = world_->currentCamera().getViewMatrix();

    Uniform<Matrix4>::set(shader.getUniform("projection"), projection);
    Uniform<Matrix4>::set(shader.getUniform("view"), view);
    for (VE::RigidBodyPtr rigidBody: world_->worldObjects) {
        for (size_t colliderNumber = 0; colliderNumber < rigidBody->collidersSize(); colliderNumber++) {
            Matrix4 model = rigidBody->transform().toMatrix();
            Uniform<Matrix4>::set(shader.getUniform("model"), model);
            Attribute<Vector3> vertexPosition;
            vertexPosition.set(rigidBody->collider(colliderNumber).vertices());
            Attribute<Vector3> vertexColor;

            std::vector<Vector3> color(rigidBody->collider(colliderNumber).vertices().size(), rigidBody->color());
            vertexColor.set(color);

            IndexBuffer indexBuffer;
            indexBuffer.set(rigidBody->collider(colliderNumber).indices());


            vertexPosition.bindTo(shader.getAttribute("position"));
            vertexColor.bindTo(shader.getAttribute("color"));

            VE::draw(indexBuffer, DrawMode::Polygon);


            vertexPosition.unBindFrom(shader.getAttribute("position"));
            vertexColor.unBindFrom(shader.getAttribute("color"));

            //drawShape(rigidBody->collider(colliderNumber), rigidBody->color());
        }
    }
    Uniform<Matrix4>::set(shader.getUniform("model"), Matrix4());

}

