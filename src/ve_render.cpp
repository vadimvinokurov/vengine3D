//
// Created by boris on 11/16/2021.
//

#include "ve_render.h"
#include "Math/ve_vector.h"
#include "Object/ve_box_shape.h"
#include "Object/ve_sphere_shape.h"

#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>


using namespace VE;

Render::Render(float windowAspectRatio) : windowAspectRatio_(windowAspectRatio) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
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

//void drawShape(const VE::Shape &shape) {
//
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 0, shape.verticesGLFormatData());
//    glPushMatrix();
////    glTranslatef(1, 1, 0);
////    glRotatef(45, 1, 0, 0);
////    glScalef(2, 2, 2);
//    for (int i = 0; i < 6; i++) {
//        glColor3f(0.50f + i / 100.0, 0.50f + i / 100.0, 0.50f + i / 100.0);
//        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, shape.indicesGLFormatData(i * 4));
//    }
//    drawAxis(2);
//    glPopMatrix();
//    glDisableClientState(GL_VERTEX_ARRAY);
//};

void drawShape(const VE::Shape &shape) {

    glBegin(GL_LINE_LOOP);
    for (const Vector &vertex: shape.vertices_) {
        glVertex3f(vertex.x(), vertex.y(), vertex.z());
    }
    glEnd();
};

void drawFlor() {
    float floorVertices[] = {1, 1, 0,
                             -1, 1, 0,
                             -1, -1, 0,
                             1, -1, 0};
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, floorVertices);
    for (int i = -5; i < 5; i++) {
        for (int j = -5; j < 5; j++) {
            glPushMatrix();
            if ((i + j) % 2) glColor3f(0, 0.5, 0);
            else glColor3f(1, 1, 1);
            glTranslated(i * 2, j * 2, 0);
            glDrawArrays(GL_POLYGON, 0, 4);
            glPopMatrix();
        }
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    drawAxis();
}

void Render::moveCamera() {
    Camera camera = world_->currentCamera();
    glRotatef(-camera.rotation().x(), 1, 0, 0);
    glRotatef(-camera.rotation().y(), 0, 1, 0);
    glRotatef(-camera.rotation().z(), 0, 0, 1);
    glTranslated(-camera.position().x(), -camera.position().y(), -camera.position().z());
}

void Render::draw(const WorldPtr &world) {
    world_ = world;


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    moveCamera();
    drawFlor();
    VE::SphereShape shape;
    drawShape(shape);
    glPopMatrix();

}

