//
// Created by boris on 8/7/2022.
//

#ifndef VENGINE3D_SYSTEMMANAGER_H
#define VENGINE3D_SYSTEMMANAGER_H

#include "ECS/Memory/Allocator/LinearAllocator.h"
#include "ECS/Memory/GlobalMemoryManager.h"
#include "ECSProperty.h"
#include "ECS/Objects/System.h"
#include <unordered_map>

class SystemManager
{
public:
	SystemManager()
	{
		allocator = LinearAllocator::create(GlobalMemoryManager::allocateMemoryPool(SYSTEM_MEMORY_BUFFER_SIZE));
	}
	template <class T, class... Args>
	T *addSystem(Args &&...args)
	{
		SystemTypeId systemTypeId = T::getTypeId();
		auto it = systems_.find(systemTypeId);
		if (it != systems_.end() && it->second != nullptr)
		{
			spdlog::warn("Multiple registrations of the same {}", typeid(T).name());
			return (T *)(it->second);
		}
		void *systemMem = allocator->allocate(sizeof(T), alignof(T));
		if (systemMem == nullptr)
		{
			assert(false && "Unable to allocate memory for system");
			return nullptr;
		}

		T *system = nullptr;
		try
		{
			system = new (systemMem) T(std::forward<Args>(args)...);
		}
		catch (std::exception &e)
		{
			spdlog::error("System constructor exception: {}", e.what());
			assert(false);
			return nullptr;
		}
		catch (...)
		{
			spdlog::error("VObject constructor exception: unknown");
			assert(false);
			return nullptr;
		}
		if (system == nullptr)
		{
			assert(false && "Unable to construct system");
			return nullptr;
		}
		system->systemManager_ = this;
		systems_[systemTypeId] = system;
		systemWorkOrder_.push_back(system);
		return system;
	}

private:
	AllocatorPtr allocator;
	std::unordered_map<SystemTypeId, ISystem *> systems_;
	std::vector<ISystem *> systemWorkOrder_;
};

#endif // VENGINE3D_SYSTEMMANAGER_H
