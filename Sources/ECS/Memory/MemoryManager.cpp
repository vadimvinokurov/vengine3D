//
// Created by boris on 7/22/2022.
//

#include "MemoryManager.h"
#include <cassert>

MemoryManager::MemoryManager(size_t chunkSize) : CHUNK_SIZE(chunkSize)
{
	memoryChunks_.emplace_back(CHUNK_SIZE);
}

void *MemoryManager::allocate(size_t size, const std::string &tag)
{
	for (auto &allocator : memoryChunks_)
	{
		auto ptr = allocator.allocate(size, alignof(uint8));
		if (ptr)
		{
			return ptr;
		}
	}
	memoryChunks_.emplace_back(CHUNK_SIZE);
	return memoryChunks_.back().allocate(size, alignof(uint8));
}

void MemoryManager::deallocate(void *ptr)
{

	auto chunkIt = std::find_if(memoryChunks_.begin(), memoryChunks_.end(),
								[ptr](const StackAllocator &allocator) { return allocator.own(ptr); });
	assert(chunkIt != memoryChunks_.end());
	if (chunkIt == memoryChunks_.end())
		return;
	chunkIt->free(ptr);
}
