//
// Created by boris on 7/21/2022.
//

#ifndef VENGINE3D_IALLOCATOR_H
#define VENGINE3D_IALLOCATOR_H

#include "EngineTypes.h"

class IAllocator
{
public:
	virtual void *allocate(size_t size = 1, uint8 alignment = 1) = 0;
	virtual void free(void *ptr) = 0;
	virtual bool own(void *ptr) const = 0;
	virtual ~IAllocator() = default;
};

#endif // VENGINE3D_IALLOCATOR_H
