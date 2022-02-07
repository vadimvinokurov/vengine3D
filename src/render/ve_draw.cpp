//
// Created by boris on 2/7/2022.
//
#include "glad/glad.h"

#include "ve_draw.h"

static GLenum drawModeToGLEnum(VE::DrawMode input){
    switch(input){
        case VE::DrawMode::Points: return GL_POINTS;
        case VE::DrawMode::LineStrip: return GL_LINE_STRIP;
        case VE::DrawMode::LineLoop: return GL_LINE_LOOP;
        case VE::DrawMode::Lines: return GL_LINES;
        case VE::DrawMode::Triangles: return GL_TRIANGLES;
        case VE::DrawMode::TriangleStrip: return GL_TRIANGLE_STRIP;
        case VE::DrawMode::TriangleFan: return GL_TRIANGLE_FAN;
    }
    std::cout << "DrawModeToGLEnum unreachable code hit\n";
    return 0;
}

void VE::draw(VE::IndexBuffer &inIndexBuffer, VE::DrawMode mode) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inIndexBuffer.getHandle());
    glDrawElements(drawModeToGLEnum(mode), inIndexBuffer.count(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VE::draw(unsigned int vertexCount, VE::DrawMode mode) {
    glDrawArrays(drawModeToGLEnum(mode), 0, vertexCount);
}

void VE::drawInstance(VE::IndexBuffer &inIndexBuffer, VE::DrawMode mode, unsigned int instanceCount) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inIndexBuffer.getHandle());
    glDrawElementsInstanced(drawModeToGLEnum(mode), inIndexBuffer.count(), GL_UNSIGNED_INT, 0, instanceCount);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VE::drawInstance(unsigned int vertexCount, VE::DrawMode mode, unsigned int numInstance) {
    glDrawArraysInstanced(drawModeToGLEnum(mode), 0, vertexCount, numInstance);
}
