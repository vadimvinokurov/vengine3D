//
// Created by boris on 8/3/2022.
//

#ifndef VENGINE3D_GLOBALMEMORYMANAGER_H
#define VENGINE3D_GLOBALMEMORYMANAGER_H

#include "MemoryManager.h"

struct GlobalMemoryManager
{
	static std::shared_ptr<MemoryPool> allocateMemoryPool(size_t size)
	{
		return memoryManager.allocateMemoryPool(size);
	}

	static void *allocate(size_t size)
	{
		return memoryManager.allocate(size);
	}

	static void free(void *ptr)
	{
		memoryManager.free(ptr);
	}

private:
	static MemoryManager memoryManager;
};

#endif // VENGINE3D_GLOBALMEMORYMANAGER_H
