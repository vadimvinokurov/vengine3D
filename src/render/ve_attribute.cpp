//
// Created by boris on 2/7/2022.
//
#include "ve_attribute.h"

#include "glad/glad.h"
#include "math/ve_vector.h"
#include "math/ve_quaternion.h"

using namespace VE;


template class VE::Attribute<int>;
template class VE::Attribute<float>;
template class VE::Attribute<Vector2>;
template class VE::Attribute<Vector3>;
template class VE::Attribute<Vector4>;
template class VE::Attribute<IVector4>;
template class VE::Attribute<Quaternion>;

template<typename T>
VE::Attribute<T>::Attribute() {
    glGenBuffers(1, &handle_);
    count_ = 0;
}

template<typename T>
VE::Attribute<T>::~Attribute() {
    glDeleteBuffers(1, &handle_);
}

template<typename T>
void VE::Attribute<T>::set(const T *inputArray, unsigned int arrayLength) {
    count_ = arrayLength;

    glBindBuffer(GL_ARRAY_BUFFER, handle_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * count_, inputArray, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void Attribute<T>::set(const std::vector<T> &input) {
    set(input.data(), input.size());
}

template<typename T>
void Attribute<T>::bindTo(unsigned int slot) {
    glBindBuffer(GL_ARRAY_BUFFER, handle_);
    setAttributePointer(slot);
    glEnableVertexAttribArray(slot);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void Attribute<T>::unBindFrom(unsigned int slot) {
    glBindBuffer(GL_ARRAY_BUFFER, handle_);
    glDisableVertexAttribArray(slot);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
unsigned int Attribute<T>::count() {
    return count_;
}

template<typename T>
unsigned int Attribute<T>::getHandle() {
    return handle_;
}

template<>
void VE::Attribute<int>::setAttributePointer(unsigned int slot) {
    glVertexAttribIPointer(slot, 1, GL_INT, 0, NULL);
}

template<>
void VE::Attribute<IVector4>::setAttributePointer(unsigned int slot) {
    glVertexAttribIPointer(slot, 4, GL_INT, 0, NULL);
}

template<>
void VE::Attribute<float>::setAttributePointer(unsigned int slot) {
    glVertexAttribPointer(slot, 1, GL_FLOAT, GL_FALSE, 0, NULL);
}

template<>
void VE::Attribute<Vector2>::setAttributePointer(unsigned int slot) {
    glVertexAttribPointer(slot, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

template<>
void VE::Attribute<Vector3>::setAttributePointer(unsigned int slot) {
    glVertexAttribPointer(slot, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

template<>
void VE::Attribute<Vector4>::setAttributePointer(unsigned int slot) {
    glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, NULL);
}

template<>
void VE::Attribute<Quaternion>::setAttributePointer(unsigned int slot) {
    glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, NULL);
}