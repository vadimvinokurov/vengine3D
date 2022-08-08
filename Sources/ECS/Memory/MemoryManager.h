//
// Created by boris on 7/22/2022.
//

#ifndef VENGINE3D_MEMORYMANAGER_H
#define VENGINE3D_MEMORYMANAGER_H

#include "ECS/Memory/Allocator/StackAllocator.h"
#include <list>
#include <string>

class MemoryManager
{
public:
	using Allocator = StackAllocator;

	MemoryManager();
	MemoryPoolPtr allocateMemoryPool(size_t size);
	void *allocate(size_t size);
	void free(void *ptr);

private:
	std::pair<void *, AllocatorPtr> allocate_implementation(size_t size);
	AllocatorPtr getAllocator(void *ptr);

	std::list<AllocatorPtr> chunks_;
};

#endif // VENGINE3D_MEMORYMANAGER_H
