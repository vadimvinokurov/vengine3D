//
// Created by boris on 11/16/2021.
//

#include "ve_render.h"
#include <iostream>

using namespace VE;


void glTranslatef(VE::Vector position) {
    glTranslatef(position.x(), position.y(), position.z());
};

void glRotatef(VE::Vector rotate) {
    auto [rotateVector, angleInRad] = rotate.getNormalAndLen();
    float angle = angleInRad * 180.0f / M_PI;
    glRotatef(angle, rotateVector.x(), rotateVector.y(), rotateVector.z());
};


Render::Render(float windowAspectRatio) : windowAspectRatio_(windowAspectRatio) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    //glClearColor(0.2f, 0.7f, 1.0f, 0.0f);
    glClearDepth(1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-windowAspectRatio_, windowAspectRatio_, -1, 1, 2, 8000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawAxis(float axisLen = 10) {
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(axisLen, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, axisLen, 0);
    glColor3f(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, axisLen);
    glEnd();
}


void drawShape(const VE::Collider &shape, const VE::Transform &transform, const Color &color) {

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, shape.verticesGLFormatData());

    glPushMatrix();
    glTranslatef(transform.position);
    glRotatef(transform.rotation.toAxisAngle());
    for (int i = 0; i < shape.indecesSize() / 4; i++) {
        glColor3f(color.red() + i / 100.0, color.grean() + i / 100.0, color.blue() + i / 100.0);
        glDrawElements(globalParameters.polygone ? GL_POLYGON : GL_LINE_LOOP, 4, GL_UNSIGNED_INT, shape.indicesGLFormatData(i * 4));
    }
    //drawAxis(2);
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
};

void drawFlor() {
    float floorVertices[] = {1, 1, 0,
                             -1, 1, 0,
                             -1, -1, 0,
                             1, -1, 0};
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, floorVertices);

    glDisableClientState(GL_VERTEX_ARRAY);
    drawAxis();
}

void Render::draw(const WorldPtr &world) {
    world_ = world;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glLoadMatrixf(world_->currentCamera().getViewMatrix().data());
    drawFlor();

    for (VE::RigidBodyPtr rigidBody: world_->worldObjects) {
        for (size_t colliderNumber = 0; colliderNumber < rigidBody->collidersSize(); colliderNumber++) {
            drawShape(rigidBody->collider(colliderNumber), rigidBody->transform(), rigidBody->color());
        }
    }

}

