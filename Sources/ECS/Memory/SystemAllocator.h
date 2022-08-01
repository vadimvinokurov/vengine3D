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
		return std::malloc(size);
	}
	virtual void free(void *ptr) override
	{
		std::free(ptr);
	}
	virtual bool own(void *ptr) const override
	{
		return true;
	}
};

#endif // VENGINE3D_SYSTEMALLOCATOR_H
