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
	for (auto &chunk : memoryChunks_)
	{
		auto ptr = chunk.allocator.allocate(size, alignof(uint8));
		if (ptr)
		{
			return ptr;
		}
	}
	memoryChunks_.emplace_back(CHUNK_SIZE);
	return memoryChunks_.back().allocator.allocate(size, alignof(uint8));
}

void MemoryManager::deallocate(void *ptr)
{
	auto chunkIt = std::find_if(memoryChunks_.begin(), memoryChunks_.end(),
								[ptr, chunkSize = CHUNK_SIZE](const MemoryChunk &chunk) {
									auto diff = (byte *)ptr - (byte *)chunk.memory_;
									return diff >= 0 & diff < chunkSize;
								});
	assert(chunkIt != memoryChunks_.end());
	if(chunkIt == memoryChunks_.end()) return;
	chunkIt->allocator.deallocate(ptr);
}
