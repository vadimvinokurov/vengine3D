//
// Created by boris on 7/22/2022.
//

#ifndef VENGINE3D_MEMORYMANAGER_H
#define VENGINE3D_MEMORYMANAGER_H

#include "StackAllocator.h"
#include <list>
#include <string>

class MemoryManager
{
private:
	struct MemoryChunk final
	{
		MemoryChunk(size_t size) : memory_(new uint8[size]), allocator(memory_, size)
		{
		}

		MemoryChunk(const MemoryChunk &other) = delete;
		MemoryChunk &operator=(const MemoryChunk &other) = delete;

		~MemoryChunk()
		{
			delete[] memory_;
		}

		uint8 *memory_;
		StackAllocator allocator;
	};

public:
	MemoryManager(size_t chunkSize);
	void *allocate(size_t size, const std::string &tag);
	void deallocate(void *ptr);

private:
	const size_t CHUNK_SIZE;
	std::list<MemoryChunk> memoryChunks_;
};

#endif // VENGINE3D_MEMORYMANAGER_H
