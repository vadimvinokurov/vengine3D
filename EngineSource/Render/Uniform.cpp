//
// Created by boris on 2/7/2022.
//

#include "Uniform.h"
#include "Math/Quat.h"
#include "Math/Vector.h"
#include "Math/Matrix4.h"

#include "Render/opengl_glad.h"

using namespace Render;

template class Render::Uniform<int32>;
template class Render::Uniform<IVector4>;
template class Render::Uniform<IVector2>;
template class Render::Uniform<float>;
template class Render::Uniform<Vector2>;
template class Render::Uniform<Vector3>;
template class Render::Uniform<Vector4>;
template class Render::Uniform<Quaternion>;
template class Render::Uniform<Matrix4>;

template <typename T>
void Uniform<T>::set(uint32 slot, const T &value)
{
	set(slot, const_cast<T *>(&value), 1);
}

template <typename T>
void Uniform<T>::set(uint32 slot, const std::vector<T> &arr)
{
	set(slot, arr.data(), static_cast<uint32>(arr.size()));
}

template <>
void Uniform<int32>::set(uint32 slot, const int32 *arr, uint32 arraySize)
{
	glUniform1iv(slot, static_cast<GLsizei>(arraySize), arr);
}

template <>
void Uniform<IVector4>::set(uint32 slot, const IVector4 *arr, uint32 arraySize)
{
	glUniform4iv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template <>
void Uniform<IVector2>::set(uint32 slot, const IVector2 *arr, uint32 arraySize)
{
	glUniform2iv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template <>
void Uniform<float>::set(uint32 slot, const float *arr, uint32 arraySize)
{
	glUniform1fv(slot, static_cast<GLsizei>(arraySize), arr);
}

template <>
void Uniform<Vector2>::set(uint32 slot, const Vector2 *arr, uint32 arraySize)
{
	glUniform2fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template <>
void Uniform<Vector3>::set(uint32 slot, const Vector3 *arr, uint32 arraySize)
{
	glUniform3fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template <>
void Uniform<Vector4>::set(uint32 slot, const Vector4 *arr, uint32 arraySize)
{
	glUniform4fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template <>
void Uniform<Quaternion>::set(uint32 slot, const Quaternion *arr, uint32 arraySize)
{
	glUniform4fv(slot, static_cast<GLsizei>(arraySize), arr->data());
}

template <>
void Uniform<Matrix4>::set(uint32 slot, const Matrix4 *arr, uint32 arraySize)
{
	glUniformMatrix4fv(slot, static_cast<GLsizei>(arraySize), false, arr->data());
}