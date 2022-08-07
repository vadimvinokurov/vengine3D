//
// Created by boris on 7/21/2022.
//

#ifndef VENGINE3D_IALLOCATOR_H
#define VENGINE3D_IALLOCATOR_H

#include "EnginePlatform.h"
#include <list>
#include <cassert>
#include <memory>

#define DEBUG_ALLOCATOR

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

#ifndef DEBUG_ALLOCATOR
	virtual ~IAllocator() = default;
#else
	virtual ~IAllocator()
	{
		assert(ptrs.empty() && "memory leak detection");
	};

protected:
	void debug_allocate(void *ptr)
	{
		if (ptr)
		{
			ptrs.push_back(ptr);
		}
	}
	void debug_free(void *ptr)
	{
		ptrs.remove(ptr);
	}
	std::list<void *> ptrs;
#endif
};

using AllocatorPtr = std::shared_ptr<IAllocator>;

#endif // VENGINE3D_IALLOCATOR_H
