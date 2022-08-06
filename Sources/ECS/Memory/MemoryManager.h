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

	MemoryManager() = default;
	std::shared_ptr<MemoryPool> allocateMemoryPool(size_t size);
	void *allocate(size_t size);
	void free(void *ptr);

private:
	std::pair<void *, std::shared_ptr<Allocator>> allocate_implementation(size_t size);
	std::shared_ptr<Allocator> getAllocator(void *ptr);

	static constexpr size_t CHUNK_SIZE = 256 * 1024 * 1024ull;
	std::list<std::shared_ptr<Allocator>> chunks_;
};

#endif // VENGINE3D_MEMORYMANAGER_H
