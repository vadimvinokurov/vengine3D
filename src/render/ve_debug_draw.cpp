//
// Created by boris on 2/12/2022.
//
#include "ve_debug_draw.h"
#include <glad/glad.h>

using namespace VE;

void VE::debugDrawVector(const Vector3& vector3, const Vector3 &basePoint, const VE::Color &color){
    glLineWidth(2);
    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_LINES);
    glVertex3f(basePoint.x, basePoint.y, basePoint.z);
    Vector3 vector = basePoint + vector3;
    glVertex3f(vector.x, vector.y, vector.z);
    glEnd();
    glPointSize(6);
    glBegin(GL_POINTS);
    glVertex3f(vector.x, vector.y, vector.z);
    glEnd();
}

void VE::debugDrawPoint(const Vector3& vector3, int size, const VE::Color &color){
    glColor3f(color.red, color.green, color.blue);
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex3f(vector3.x, vector3.y, vector3.z);
    glEnd();
}
