//
// Created by boris on 8/2/2022.
//

#ifndef VENGINE3D_MEMORYPOOL_H
#define VENGINE3D_MEMORYPOOL_H

#include <memory>
#include <iostream>
#include <EngineTypes.h>
#include "SystemAllocator.h"
#include "spdlog/spdlog.h"
struct MemoryPool
{
	MemoryPool(size_t sz, const std::shared_ptr<IAllocator> &alloc = std::make_shared<SystemAllocator>())
		: allocator(alloc), size(sz), used(0)
	{
		address = allocator->allocate(size, 1);
		if(!address) {
			spdlog::critical("Can't allocate memory pool");
			throw std::bad_alloc();
		}
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

	std::shared_ptr<IAllocator> allocator;
	size_t size;
	size_t used;
	union {
		void *address;
		uptr addressUptr;
	};
};

#endif // VENGINE3D_MEMORYPOOL_H
