//
// Created by boris on 7/22/2022.
//

#include "MemoryManager.h"
#include <cassert>

void *MemoryManager::allocate(size_t size)
{
	return allocate_implementation(size).first;
}

std::shared_ptr<MemoryPool> MemoryManager::allocateMemoryPool(size_t size)
{
	auto [ptr, allocator] = allocate_implementation(size);
	allocator->free(ptr);
	//auto p = new MemoryPool(ptr, size, allocator);
	//return std::shared_ptr<MemoryPool>(p);
	//return std::make_shared<MemoryPool>(ptr, size, allocator);
	return {};
}

std::pair<void *, const std::shared_ptr<MemoryManager::Allocator> &> MemoryManager::allocate_implementation(size_t size)
{
	for (const auto &allocator : chunks_)
	{
		auto ptr = allocator->allocate(size, alignof(uint8));
		if (ptr)
		{
			return {ptr, allocator};
		}
	}
	auto mpool = std::make_shared<MemoryPool>(std::max(size, CHUNK_SIZE));
	auto allocator = std::make_shared<Allocator>(mpool);
	auto ptr = allocator->allocate(size, alignof(uint8));
	assert(ptr != nullptr && "Bad alloc in memory manager");
	chunks_.push_back(allocator);
	return {ptr, allocator};
}

void MemoryManager::free(void *ptr)
{
	auto allocator = getAllocator(ptr);
	if (allocator.get() == nullptr)
	{
		return;
	}
	allocator->free(ptr);
}
std::shared_ptr<MemoryManager::Allocator> MemoryManager::getAllocator(void *ptr)
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
