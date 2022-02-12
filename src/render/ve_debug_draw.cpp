//
// Created by boris on 2/12/2022.
//
#include "ve_debug_draw.h"
#include "ve_uniform.h"
#include <glad/glad.h>

using namespace VE;

unsigned int DebugDraw::colorShaderSlot = static_cast<unsigned int>(-1);
void DebugDraw::Vector(const Vector3 &vector3, const Vector3 &basePoint, const Color &color) {
    glLineWidth(2);
    if(colorShaderSlot != static_cast<unsigned int>(-1)){
        Uniform<Vector3>::set(colorShaderSlot, Vector3(color.v));
    }
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

void DebugDraw::Point(const Vector3 &vector3, int size, const Color &color) {
    if(colorShaderSlot != static_cast<unsigned int>(-1)){
        Uniform<Vector3>::set(colorShaderSlot, Vector3(color.v));
    }
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex3f(vector3.x, vector3.y, vector3.z);
    glEnd();
}

void DebugDraw::Line(const Vector3 &A, const Vector3 &B, const Color &color) {
    DebugDraw::Vector(B - A, A, color);
}
