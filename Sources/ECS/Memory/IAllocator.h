//
// Created by boris on 7/21/2022.
//

#ifndef VENGINE3D_IALLOCATOR_H
#define VENGINE3D_IALLOCATOR_H

#include "EngineTypes.h"

class IAllocator
{
public:
	IAllocator(void *memory, size_t size) : baseAddress_(memory), maxSize_(size), usedMemory_(0)
	{
		static_assert(sizeof(uptr) == sizeof(void *) && "Pointer type size do not match.");
	};

	virtual ~IAllocator() = default;

	virtual void *allocate(size_t size, uint8 alignment) = 0;
	virtual void deallocate(void *ptr) = 0;
	virtual void clear() = 0;

protected:
	void *const baseAddress_;
	const size_t maxSize_;
	size_t usedMemory_;
};

#endif // VENGINE3D_IALLOCATOR_H
