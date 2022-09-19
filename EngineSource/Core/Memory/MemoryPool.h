//
// Created by boris on 8/2/2022.
//

#ifndef VENGINE3D_MEMORYPOOL_H
#define VENGINE3D_MEMORYPOOL_H

#include "Core/Memory/Allocator/SystemAllocator.h"

#include <memory>
#include "EnginePlatform.h"

#include "spdlog/spdlog.h"

struct MemoryPool
{
	using pointer = std::unique_ptr<MemoryPool>;

	static pointer create(size_t sz)
	{
		auto alloc = SystemAllocator::create();
		return create(sz, alloc);
	};

	static pointer create(size_t sz, const AllocatorPtr &alloc)
	{
		void *ptr = alloc->allocate(sz, 1);
		return create(ptr, sz, alloc);
	};

	static pointer create(void *ptr, size_t sz, const AllocatorPtr &alloc)
	{
		if (!ptr)
		{
			spdlog::critical("Can't allocate memory pool. Allocator - {}", typeid(*alloc).name());
			throw std::bad_alloc();
		}
		return std::unique_ptr<MemoryPool>(new MemoryPool(ptr, sz, alloc));
	};

	bool own(void *ptr) const
	{
		return (uptr)ptr >= addressUptr && (uptr)ptr < addressUptr + size;
	}

	MemoryPool(const MemoryPool &other) = delete;
	MemoryPool &operator=(const MemoryPool &other) = delete;
	MemoryPool(MemoryPool &&other) = delete;
	MemoryPool &operator=(MemoryPool &&other) = delete;

	~MemoryPool()
	{
		allocator->free(address);
	}

	AllocatorPtr allocator;
	union {
		void *address;
		uptr addressUptr;
	};
	size_t size;
	size_t used = 0;
private:
	MemoryPool(void *ptr, size_t sz, const AllocatorPtr &alloc)
		: allocator(alloc), address(ptr), size(sz){};
};

using MemoryPoolPtr = MemoryPool::pointer;

#endif // VENGINE3D_MEMORYPOOL_H
