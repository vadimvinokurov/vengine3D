//
// Created by boris on 7/22/2022.
//

#include "MemoryManager.h"
MemoryManager::MemoryManager(size_t chunkSize) : CHUNK_SIZE(chunkSize)
{
	memoryChunks_.emplace_back(CHUNK_SIZE);
}

void *MemoryManager::allocate(size_t size, const std::string &tag)
{
	void *ptr = nullptr;
	for (auto &chunk : memoryChunks_)
	{
		ptr = chunk.allocator.allocate(size, alignof(uint8));
	}
	if (!ptr)
	{
		memoryChunks_.emplace_back(CHUNK_SIZE);
		ptr = memoryChunks_.back().allocator.allocate(size, alignof(uint8));
	}
	return ptr;
}

void MemoryManager::deallocate(void *ptr)
{
}
