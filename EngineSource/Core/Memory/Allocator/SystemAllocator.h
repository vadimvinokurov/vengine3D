//
// Created by boris on 8/2/2022.
//

#ifndef VENGINE3D_SYSTEMALLOCATOR_H
#define VENGINE3D_SYSTEMALLOCATOR_H

#include "IAllocator.h"
#include <memory>

struct SystemAllocator final : public IAllocator
{
	static AllocatorPtr create()
	{
		return AllocatorPtr(new SystemAllocator());
	};

	void *allocate(size_t size, uint8 alignment) override
	{
		assert(alignment == 1);
		auto ptr = std::malloc(size);
#ifdef ECS_DEBUG
		debug_allocate(ptr);
#endif
		return ptr;
	}
	void free(void *ptr) override
	{
		if (!ptr)
		{
			return;
		}
#ifdef ECS_DEBUG
		debug_free(ptr);
#endif
		std::free(ptr);
	}
	bool own(void *ptr) const override
	{
		return true;
	}

private:
	SystemAllocator() = default;
};

#endif // VENGINE3D_SYSTEMALLOCATOR_H
