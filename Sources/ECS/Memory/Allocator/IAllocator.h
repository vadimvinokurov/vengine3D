//
// Created by boris on 7/21/2022.
//

#ifndef VENGINE3D_IALLOCATOR_H
#define VENGINE3D_IALLOCATOR_H

#include "EnginePlatform.h"
#include <set>
#include <cassert>
#include <memory>

class IAllocator
{
public:
	IAllocator() = default;
	IAllocator(const IAllocator &other) = delete;
	IAllocator &operator=(const IAllocator &other) = delete;
	IAllocator(IAllocator &&other) = delete;
	IAllocator &operator=(IAllocator &&other) = delete;

	virtual void *allocate(size_t size, uint8 alignment) = 0;
	virtual void *allocate() = 0;
	virtual void free(void *ptr) = 0;
	virtual bool own(void *ptr) const = 0;

#ifndef ECS_DEBUG
	virtual ~IAllocator() = default;
#else
	virtual ~IAllocator()
	{
		assert(ptrs.empty() && "memory leak detection");
	};

	void debug_allocate(void *ptr)
	{
		if (ptr)
		{
			ptrs.insert(ptr);
		}
	}
	void debug_free(void *ptr)
	{
		assert(ptrs.find(ptr) != ptrs.end() && "Try to free incorrect ptr.");
		ptrs.erase(ptr);
	}
	std::set<void *> ptrs;
#endif
};

using AllocatorPtr = std::shared_ptr<IAllocator>;

#endif // VENGINE3D_IALLOCATOR_H
