//
// Created by boris on 7/21/2022.
//

#ifndef VENGINE3D_STACKALLOCATOR_H
#define VENGINE3D_STACKALLOCATOR_H

#include "IAllocator.h"

class StackAllocator : public IAllocator
{
private:
	struct MetaInfo
	{
		uint8 adjustment;
	};

public:
	StackAllocator(void *memory, size_t size);
	void *allocate(size_t size, uint8 alignment) override;
	void deallocate(void *ptr) override;
	void clear() override;
};

#endif // VENGINE3D_STACKALLOCATOR_H
