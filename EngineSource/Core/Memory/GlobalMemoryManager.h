//
// Created by boris on 8/3/2022.
//

#ifndef VENGINE3D_GLOBALMEMORYMANAGER_H
#define VENGINE3D_GLOBALMEMORYMANAGER_H

#include "Core/Memory/Allocator/ChunkAllocator.h"

struct GlobalMemoryManager
{
	static MemoryPoolPtr allocateMemoryPool(size_t size)
	{
		auto ptr = memoryManager->allocate(size, 1);
		return MemoryPool::create(ptr, size, memoryManager);
	}

	static const AllocatorPtr &get()
	{
		return memoryManager;
	}

private:
	static AllocatorPtr memoryManager;
};

#endif // VENGINE3D_GLOBALMEMORYMANAGER_H
