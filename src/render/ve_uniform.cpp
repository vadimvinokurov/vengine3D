//
// Created by boris on 2/7/2022.
//

#include "ve_uniform.h"
#include "glad/glad.h"
#include "math/ve_quaternion.h"
#include "math/ve_vector.h"
#include "math/ve_matrix4.h"

using namespace VE;

template class VE::Uniform<int>;
template class VE::Uniform<IVector4>;
template class VE::Uniform<IVector2>;
template class VE::Uniform<float>;
template class VE::Uniform<Vector2>;
template class VE::Uniform<Vector3>;
template class VE::Uniform<Vector4>;
template class VE::Uniform<Quaternion>;
template class VE::Uniform<Matrix4>;

template<typename T>
void Uniform<T>::set(unsigned int slot, const T &value) {
    set(slot, const_cast<T*>(&value), 1);
}

template<typename T>
void Uniform<T>::set(unsigned int slot, const std::vector<T> &arr) {
    set(slot, arr.data(), static_cast<unsigned int>(arr.size()));
}

template<>
void Uniform<int>::set(unsigned int slot, const int *arr, unsigned int arraySize) {
    glUniform1iv(slot, static_cast<GLsizei>(arraySize), arr);
}

template<>
void Uniform<IVector4>::set(unsigned int slot, const IVector4 *arr, unsigned int arraySize) {
    glUniform4iv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<IVector2>::set(unsigned int slot, const IVector2 *arr, unsigned int arraySize) {
    glUniform2iv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<float>::set(unsigned int slot, const float *arr, unsigned int arraySize) {
    glUniform1fv(slot, static_cast<GLsizei>(arraySize), arr);
}

template<>
void Uniform<Vector2>::set(unsigned int slot, const Vector2 *arr, unsigned int arraySize) {
    glUniform2fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<Vector3>::set(unsigned int slot, const Vector3 *arr, unsigned int arraySize) {
    glUniform3fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<Vector4>::set(unsigned int slot, const Vector4 *arr, unsigned int arraySize) {
    glUniform4fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<Quaternion>::set(unsigned int slot, const Quaternion *arr, unsigned int arraySize) {
    glUniform4fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template<>
void Uniform<Matrix4>::set(unsigned int slot, const Matrix4 *arr, unsigned int arraySize) {
    glUniformMatrix4fv(slot, static_cast<GLsizei>(arraySize), false, arr->data());
}