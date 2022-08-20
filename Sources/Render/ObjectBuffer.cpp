//
// Created by boris on 2/7/2022.
//
#include "ObjectBuffer.h"
#include "Math/Vector.h"
#include "Math/Quat.h"
#include "Render/opengl_glad.h"

template class Render::ObjectBuffer<int32>;
template class Render::ObjectBuffer<float>;
template class Render::ObjectBuffer<Vector2>;
template class Render::ObjectBuffer<Vector3>;
template class Render::ObjectBuffer<Vector4>;
template class Render::ObjectBuffer<IVector4>;
template class Render::ObjectBuffer<Quaternion>;

using namespace Render;

template <typename T>
ObjectBuffer<T>::ObjectBuffer()
{
	glGenBuffers(1, &handle_);
	count_ = 0;
}

template <typename T>
ObjectBuffer<T>::ObjectBuffer(ObjectBuffer &&other)
{
	this->count_ = other.count_;
	this->handle_ = other.handle_;

	other.count_ = 0;
	other.handle_ = 0;
}

template <typename T>
ObjectBuffer<T> &ObjectBuffer<T>::operator=(ObjectBuffer &&other)
{
	this->count_ = other.count_;
	this->handle_ = other.handle_;

	other.count_ = 0;
	other.handle_ = 0;
	return *this;
}

template <typename T>
ObjectBuffer<T>::~ObjectBuffer()
{
	if (handle_ == 0)
	{
		return;
	}
	glDeleteBuffers(1, &handle_);
}

template <typename T>
void ObjectBuffer<T>::set(const T *inputArray, uint32 arrayLength)
{
	count_ = arrayLength;

	glBindBuffer(GL_ARRAY_BUFFER, handle_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(T) * count_, inputArray, GL_STATIC_DRAW);
}

template <typename T>
void ObjectBuffer<T>::set(const std::vector<T> &input)
{
	if (input.empty())
		return;
	set(input.data(), input.size());
}

template <typename T>
void ObjectBuffer<T>::attachToAttribute(uint32 slot)
{
	glBindBuffer(GL_ARRAY_BUFFER, handle_);
	setAttributePointer(slot);
	glEnableVertexAttribArray(slot);
}

template <typename T>
void ObjectBuffer<T>::detachFromAttribute(uint32 slot)
{
	glBindBuffer(GL_ARRAY_BUFFER, handle_);
	glDisableVertexAttribArray(slot);
}

template <typename T>
unsigned int ObjectBuffer<T>::count()
{
	return count_;
}

template <typename T>
unsigned int ObjectBuffer<T>::getHandle()
{
	return handle_;
}

template <>
void ObjectBuffer<int>::setAttributePointer(uint32 slot)
{
	glVertexAttribIPointer(slot, 1, GL_INT, 0, NULL);
}

template <>
void ObjectBuffer<IVector4>::setAttributePointer(uint32 slot)
{
	glVertexAttribIPointer(slot, 4, GL_INT, 0, NULL);
}

template <>
void ObjectBuffer<float>::setAttributePointer(uint32 slot)
{
	glVertexAttribPointer(slot, 1, GL_FLOAT, GL_FALSE, 0, NULL);
}

template <>
void ObjectBuffer<Vector2>::setAttributePointer(uint32 slot)
{
	glVertexAttribPointer(slot, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

template <>
void ObjectBuffer<Vector3>::setAttributePointer(uint32 slot)
{
	glVertexAttribPointer(slot, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

template <>
void ObjectBuffer<Vector4>::setAttributePointer(uint32 slot)
{
	glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, NULL);
}

template <>
void ObjectBuffer<Quaternion>::setAttributePointer(uint32 slot)
{
	glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, NULL);
}