//
// Created by boris on 8/3/2022.
//

#ifndef VENGINE3D_OBJECTMANAGER_H
#define VENGINE3D_OBJECTMANAGER_H

#include "GlobalMemoryManager.h"
#include "Allocator/BlockAllocator.h"
#include <list>

template <typename T, size_t CHUNK_SIZE>
class ObjectManager
{
private:
	using Allocator = BlockAllocator;
	static constexpr auto CHUNK_MEMORY_SIZE = CHUNK_SIZE * (sizeof(T) + alignof(T));

public:
	ObjectManager(){
		auto mpool = GlobalMemoryManager::allocateMemoryPool(CHUNK_MEMORY_SIZE);
		chunks.push_back(std::make_shared<Allocator>(mpool, sizeof(T), alignof(T)));
	}

	template <typename... Args>
	T *CreateObject(Args &&...args);

	void DestroyObject(T *objectPtr);

private:
	std::list<std::shared_ptr<Allocator>> chunks;
};
#endif // VENGINE3D_OBJECTMANAGER_H
