//
// Created by boris on 2/7/2022.
//

#include "ve_uniform.h"

using namespace VE;

template<>
void Uniform<int>::set(unsigned int slot, int *arr, unsigned int arraySize) {
    glUniform1iv(slot, static_cast<GLsizei>(arraySize), arr);
}

template<>
void Uniform<IVector4>::set(unsigned int slot, IVector4 *arr, unsigned int arraySize) {
    glUniform4iv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<IVector2>::set(unsigned int slot, IVector2 *arr, unsigned int arraySize) {
    glUniform2iv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<float>::set(unsigned int slot, float *arr, unsigned int arraySize) {
    glUniform1fv(slot, static_cast<GLsizei>(arraySize), arr);
}

template<>
void Uniform<Vector2>::set(unsigned int slot, Vector2 *arr, unsigned int arraySize) {
    glUniform2fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<Vector3>::set(unsigned int slot, Vector3 *arr, unsigned int arraySize) {
    glUniform3fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<Vector4>::set(unsigned int slot, Vector4 *arr, unsigned int arraySize) {
    glUniform4fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<Quaternion>::set(unsigned int slot, Quaternion *arr, unsigned int arraySize) {
    glUniform4fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<Matrix4>::set(unsigned int slot, Matrix4 *arr, unsigned int arraySize) {
    glUniformMatrix4fv(slot, static_cast<GLsizei>(arraySize), false, arr->data());
}