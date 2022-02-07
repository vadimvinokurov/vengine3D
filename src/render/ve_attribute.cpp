//
// Created by boris on 2/7/2022.
//

#include "ve_attribute.h"


using namespace VE;

template<>
void VE::Attribute<int>::setAttributePointer(unsigned int slot) {
    glVertexAttribIPointer(slot, 1, GL_INT, 0, (void *) 0);
}

template<>
void VE::Attribute<IVector4>::setAttributePointer(unsigned int slot) {
    glVertexAttribIPointer(slot, 4, GL_INT, 0, (void *) 0);
}

template<>
void VE::Attribute<float>::setAttributePointer(unsigned int slot) {
    glVertexAttribIPointer(slot, 1, GL_FLOAT, 0, 0);
}

template<>
void VE::Attribute<Vector2>::setAttributePointer(unsigned int slot) {
    glVertexAttribIPointer(slot, 2, GL_FLOAT, 0, 0);
}

template<>
void VE::Attribute<Vector3>::setAttributePointer(unsigned int slot) {
    glVertexAttribIPointer(slot, 3, GL_FLOAT, 0, 0);
}

template<>
void VE::Attribute<Vector4>::setAttributePointer(unsigned int slot) {
    glVertexAttribIPointer(slot, 4, GL_FLOAT, 0, 0);
}