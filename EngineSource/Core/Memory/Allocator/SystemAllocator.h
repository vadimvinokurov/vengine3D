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

	virtual void *allocate() override
	{
		assert(false && "This method is not supported.");
		return nullptr;
	}

	virtual void *allocate(size_t size, uint8 alignment = 1) override
	{
		auto ptr = std::malloc(size);
#ifdef ECS_DEBUG
		debug_allocate(ptr);
#endif
		return ptr;
	}
	virtual void free(void *ptr) override
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
	virtual bool own(void *ptr) const override
	{
		return true;
	}

private:
	SystemAllocator() = default;
};

#endif // VENGINE3D_SYSTEMALLOCATOR_H
