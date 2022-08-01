//
// Created by boris on 8/2/2022.
//

#ifndef VENGINE3D_MEMORYPOOL_H
#define VENGINE3D_MEMORYPOOL_H

#include <memory>
#include <iostream>
#include <EngineTypes.h>
#include "SystemAllocator.h"
struct MemoryPool
{
	MemoryPool(size_t sz, const std::shared_ptr<IAllocator> &alloc = std::make_shared<SystemAllocator>())
		: allocator(alloc), size(sz), used(0)
	{
		address = allocator->allocate(size, 1);
	};

	bool own(void *ptr) const
	{
		return (uptr)ptr >= addressUptr && (uptr)ptr < addressUptr + size;
	}

	MemoryPool(const MemoryPool &other) = delete;
	MemoryPool &operator=(const MemoryPool &other) = delete;

	MemoryPool(MemoryPool &&other)
	{
		allocator = other.allocator;
		size = other.size;
		used = other.used;
		address = other.address;

		other.size = 0;
		other.used = 0;
		other.address = nullptr;
	}
	MemoryPool &operator=(MemoryPool &&other)
	{
		allocator = other.allocator;
		size = other.size;
		used = other.used;
		address = other.address;

		other.size = 0;
		other.used = 0;
		other.address = nullptr;
	}

	~MemoryPool()
	{
		if (address)
		{
			allocator->free(address);
		}

		size = 0;
		used = 0;
		address = nullptr;
	}

	void set(uint8 value)
	{
		std::memset(address, value, size);
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
