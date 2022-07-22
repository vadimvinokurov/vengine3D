//
// Created by boris on 7/21/2022.
//

#ifndef VENGINE3D_STACKALLOCATOR_H
#define VENGINE3D_STACKALLOCATOR_H

#include "IAllocator.h"
#include <vector>

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
private:
	void *allocate_memory(size_t size, uint8 alignment);
	void deallocate_memory(void *ptr);

	std::vector<void *> issuedMemory_;
	std::vector<void *> freedMemory_;
};

#endif // VENGINE3D_STACKALLOCATOR_H
