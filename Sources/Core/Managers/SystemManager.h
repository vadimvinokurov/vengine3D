//
// Created by boris on 8/7/2022.
//

#ifndef VENGINE3D_SYSTEMMANAGER_H
#define VENGINE3D_SYSTEMMANAGER_H

#include "Core/Memory/Allocator/LinearAllocator.h"
#include "Core/Memory/GlobalMemoryManager.h"
#include "EngineProperty.h"
#include "Core/Objects/System.h"
#include <unordered_map>

class SystemManager
{
public:
	SystemManager()
	{
		allocator = LinearAllocator::create(GlobalMemoryManager::allocateMemoryPool(SYSTEM_MEMORY_BUFFER_SIZE));
	}

	SystemManager(const SystemManager &) = delete;
	SystemManager &operator=(const SystemManager &) = delete;

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

		if (systemTypeId >= systemDependency_.size())
		{
			systemDependency_.resize(systemTypeId + 1);
			for (auto &sd : systemDependency_)
			{
				sd.resize(systemTypeId + 1, false);
			}
		}

		return system;
	}

	template <class System, class Dependency>
	void setSystemDependency()
	{
		systemDependency_[System::getTypeId()][Dependency::getTypeId()] = true;
	}

	template <class System, class Dependency1, class Dependency2, class... Dependencies>
	void setSystemDependency()
	{
		systemDependency_[System::getTypeId()][Dependency1::getTypeId()] = true;
		setSystemDependency<System, Dependency2, Dependencies...>();
	}

	template <class T>
	T *getSystem() const
	{
		auto it = systems_.find(T::getTypeId());
		return it != systems_.end() ? (T *)(it->second) : nullptr;
	}

	template <class T>
	void enableSystem()
	{
		auto it = systems_.find(T::getTypeId());
		if (it != systems_.end() && it->second != nullptr)
		{
			it->second->enabled_ = true;
		}
		else
		{
			spdlog::warn("Trying to enable system - {}, but system is not registered yet.", typeid(T).name());
		}
	}

	template <class T>
	void disableSystem()
	{
		auto it = systems_.find(T::getTypeId());
		if (it != systems_.end() && it->second != nullptr)
		{
			it->second->enabled_ = false;
		}
		else
		{
			spdlog::warn("Trying to disable system - {}, but system is not registered yet.", typeid(T).name());
		}
	}

	template <class T>
	void setSystemUpdateInterval(float interval)
	{
		auto it = systems_.find(T::getTypeId());
		if (it != systems_.end() && it->second != nullptr)
		{
			it->second->updateInterval_ = interval;
		}
		else
		{
			spdlog::warn("Trying to set update interval - {}, but system is not registered yet.", typeid(T).name());
		}
	}
	template <class T>
	void setSystemPriority(SystemPriority newPriority)
	{
		auto it = systems_.find(T::getTypeId());
		if (it != systems_.end() && it->second != nullptr)
		{
			it->second->priority_ = newPriority;
		}
		else
		{
			spdlog::warn("Trying to set priority - {}, but system is not registered yet.", typeid(T).name());
		}
	}

	void updateSystemWorkOrder()
	{
		const auto numberOfSystems = systemWorkOrder_.size();

		std::vector<SystemTypeId> unorderedSystems(numberOfSystems);
		for (size_t i = 0; i < unorderedSystems.size(); ++i)
		{
			unorderedSystems[i] = i;
		}
		std::vector<std::pair<SystemPriority, std::vector<SystemTypeId>>> dependencyGroups;

		while (!unorderedSystems.empty())
		{
			auto index = unorderedSystems.back();
			unorderedSystems.pop_back();
			if (index == INVALID_ID)
			{
				continue;
			}

			std::vector<SystemTypeId> member;
			member.push_back(index);

			SystemPriority groupPriority = System::LOWEST_SYSTEM_PRIORITY;
			std::vector<SystemTypeId> group;

			while (!member.empty())
			{
				index = member.back();
				member.pop_back();

				for (auto &unorderedSystem : unorderedSystems)
				{
					if (unorderedSystem != INVALID_ID &&
						(systemDependency_[index][unorderedSystem] || systemDependency_[unorderedSystem][index]))
					{
						member.push_back(unorderedSystem);
						unorderedSystem = INVALID_ID;
					}
				}
				System *system = systems_[index];
				if (!system)
				{
					continue;
				}
				group.push_back(index);
				groupPriority = std::max(system->priority_, groupPriority);
			}
			dependencyGroups.emplace_back(groupPriority, std::move(group));
		}

		std::function<void(size_t, std::vector<bool> &, std::vector<SystemTypeId> &)> DFS =
			[&](size_t vertex, std::vector<bool> &verticesVisited, std::vector<SystemTypeId> &order) {
				verticesVisited[vertex] = true;
				for (size_t i = 0; i < verticesVisited.size(); ++i)
				{
					if (!verticesVisited[i] && systemDependency_[vertex][i])
					{
						DFS(i, verticesVisited, order);
					}
				}
				order.push_back(vertex);
			};

		std::multimap<SystemPriority, std::vector<SystemTypeId>> sortedDependencyGroups;
		std::vector<bool> verticesVisited(numberOfSystems, false);
		for (auto &group : dependencyGroups)
		{
			std::vector<SystemTypeId> order;
			for (auto &id : group.second)
			{
				if (!verticesVisited[id])
				{
					DFS(id, verticesVisited, order);
				}
			}
			sortedDependencyGroups.emplace(System::HIGHEST_SYSTEM_PRIORITY - group.first, order);
		}

		systemWorkOrder_.clear();
		for (auto &group : sortedDependencyGroups)
		{
			for (auto &id : group.second)
			{
				System *system = systems_[id];
				if (system)
				{
					systemWorkOrder_.push_back(system);
				}
			}
		}
	}

	// private:
	void update(float dt)
	{
		for (System *system : systemWorkOrder_)
		{
			system->timeSinceLastUpdate_ += dt;
			system->needsUpdate_ =
				(system->updateInterval_ < 0.0f) ||
				((system->updateInterval_ > 0.0f) && (system->timeSinceLastUpdate_ > system->updateInterval_));
			if (system->needsUpdate_ && system->enabled_)
			{
				system->preUpdate(dt);
			}
		}
		for (System *system : systemWorkOrder_)
		{
			if (system->needsUpdate_ && system->enabled_)
			{
				system->update(dt);
				system->timeSinceLastUpdate_ = 0.0f;
			}
		}
		for (System *system : systemWorkOrder_)
		{
			if (system->needsUpdate_ && system->enabled_)
			{
				system->postUpdate(dt);
			}
		}
	}
	AllocatorPtr allocator;
	std::unordered_map<SystemTypeId, System *> systems_;
	std::vector<System *> systemWorkOrder_;
	std::vector<std::vector<bool>> systemDependency_;
};

#endif // VENGINE3D_SYSTEMMANAGER_H
