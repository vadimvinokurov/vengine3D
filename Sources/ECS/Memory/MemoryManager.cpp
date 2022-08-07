//
// Created by boris on 7/22/2022.
//

#include "MemoryManager.h"
#include "ECS/ECSProperty.h"
#include <cassert>

void *MemoryManager::allocate(size_t size)
{
	return allocate_implementation(size).first;
}

MemoryPoolPtr MemoryManager::allocateMemoryPool(size_t size)
{
	auto [ptr, allocator] = allocate_implementation(size);
	return MemoryPool::create(ptr, size, allocator);
}

std::pair<void *, AllocatorPtr> MemoryManager::allocate_implementation(size_t size)
{
	for (const auto &allocator : chunks_)
	{
		auto ptr = allocator->allocate(size, alignof(uint8));
		if (ptr)
		{
			return {ptr, allocator};
		}
	}
	auto mpool = MemoryPool::create(std::max(size * 2, MEMORY_MANAGER_CHUNK_SIZE));
	auto allocator = Allocator::create(std::move(mpool));
	auto ptr = allocator->allocate(size, alignof(uint8));
	assert(ptr != nullptr && "Memory manager can't allocate memory. Out of memory!");
	chunks_.push_back(allocator);
	return {ptr, allocator};
}

void MemoryManager::free(void *ptr)
{
	auto allocator = getAllocator(ptr);
	if (!allocator)
	{
		return;
	}
	allocator->free(ptr);
}
AllocatorPtr MemoryManager::getAllocator(void *ptr)
{
	for (const auto &allocator : chunks_)
	{
		if (allocator->own(ptr))
		{
			return allocator;
		}
	}
	assert(false && "MemoryManager has not this ptr");
	return {};
}
