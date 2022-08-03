//
// Created by boris on 7/22/2022.
//

#include "MemoryManager.h"
#include <cassert>

MemoryManager::MemoryManager()
{

	memoryChunks_.emplace_back(std::make_shared<MemoryPool>(CHUNK_SIZE));
}

void *MemoryManager::allocate(size_t size, uint8)
{
	for (auto &allocator : memoryChunks_)
	{
		auto ptr = allocator.allocate(size, alignof(uint8));
		if (ptr)
		{
			return ptr;
		}
	}
	memoryChunks_.emplace_back(std::make_shared<MemoryPool>(CHUNK_SIZE));
	return memoryChunks_.back().allocate(size, alignof(uint8));
}

void MemoryManager::free(void *ptr)
{
	auto chunkIt = std::find_if(memoryChunks_.begin(), memoryChunks_.end(),
								[ptr](const StackAllocator &allocator) { return allocator.own(ptr); });
	assert(chunkIt != memoryChunks_.end());
	if (chunkIt == memoryChunks_.end())
		return;
	chunkIt->free(ptr);
}
bool MemoryManager::own(void *ptr) const
{
	auto chunkIt = std::find_if(memoryChunks_.begin(), memoryChunks_.end(),
								[ptr](const StackAllocator &allocator) { return allocator.own(ptr); });
	return chunkIt != memoryChunks_.end();
}
