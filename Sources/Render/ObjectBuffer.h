//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_ATTRIBUTE_H
#define VENGINE3D_VE_ATTRIBUTE_H

#include <vector>
#include "EnginePlatform.h"

namespace Render
{
	template <typename T>
	class ObjectBuffer
	{
	public:
		ObjectBuffer(const ObjectBuffer &other) = delete;
		ObjectBuffer &operator=(const ObjectBuffer &other) = delete;

		ObjectBuffer(ObjectBuffer &&other);
		ObjectBuffer &operator=(ObjectBuffer &&other);

		ObjectBuffer();
		~ObjectBuffer();

		void set(const T *inputArray, uint32 arrayLength);
		void set(const std::vector<T> &input);

		void attachToAttribute(uint32 slot) const;
		void detachFromAttribute(uint32 slot) const;

		uint32 count() const;
		uint32 getHandle() const;

	private:
		void setAttributePointer(uint32 slot) const;

	protected:
		uint32 handle_;
		uint32 count_;
	};
} // namespace Render

#endif // VENGINE3D_VE_ATTRIBUTE_H
