//
// Created by boris on 8/2/2022.
//

#ifndef VENGINE3D_SYSTEMALLOCATOR_H
#define VENGINE3D_SYSTEMALLOCATOR_H

#include "IAllocator.h"
#include <memory>

struct SystemAllocator final : public IAllocator
{
	virtual void *allocate(size_t size, uint8 alignment = 1) override
	{
		auto ptr = std::malloc(size);
#ifdef DEBUG_ALLOCATOR
		std::memset(ptr, 0xAA, size);
		if (ptr)
		{
			debug_allocate(ptr);
		}
#endif
		return ptr;
	}
	virtual void free(void *ptr) override
	{
#ifdef DEBUG_ALLOCATOR
		debug_free(ptr);
#endif
		std::free(ptr);
	}
	virtual bool own(void *ptr) const override
	{
		return true;
	}
};

#endif // VENGINE3D_SYSTEMALLOCATOR_H
