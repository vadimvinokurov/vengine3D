//
// Created by boris on 8/7/2022.
//

#ifndef VENGINE3D_COMPONENTMANAGER_H
#define VENGINE3D_COMPONENTMANAGER_H

#include "Core/Memory/VObjectContainer.h"
#include "Core/Objects/Component.h"
#include "Core/CoreProperty.h"
#include <unordered_map>

class ComponentManager
{
private:
	template <class T>
	using ComponentContainer = VObjectContainer<T, COMPONENT_CHUNK_SIZE>;
	using IComponentContainer = IVObjectContainer;
	using ContainersMap = std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentContainer>>;

public:
	template <class T>
	using ComponentIterator = typename ComponentContainer<T>::iterator;

	ComponentManager() = default;
	ComponentManager(const ComponentManager &) = delete;
	ComponentManager &operator=(const ComponentManager &) = delete;

	template <class T, class... Args>
	T *addComponent(const EntityId entityId, Args &&...args)
	{
		static constexpr std::hash<uint64> entityComponentIdHasher{std::hash<uint64>()};
		T *component = getComponentContainer<T>()->createObject(std::forward<Args>(args)...);
		if (!component)
		{
			return nullptr;
		}

		ComponentId componentId = aqcuireComponentId(component);
		component->id_ = componentId;
		component->owner_ = entityId;
		component->hash_ = entityComponentIdHasher(entityId) ^ (entityComponentIdHasher(componentId) << 1);

		mapEntityComponent(entityId, componentId, T::getTypeId());

		return component;
	}

	template <class T>
	void removeComponent(EntityId entityId)
	{
		auto entityIdIndex = getIdIndex(entityId);
		if (entityIdIndex >= entityComponentMap_.size())
		{
			assert(false && "Entity has not any components");
			return;
		}

		auto &entityComponents = entityComponentMap_[entityIdIndex];
		ComponentTypeId componentTypeId = T::getTypeId();
		if (componentTypeId >= entityComponents.size())
		{
			assert(false && "Entity has not component");
			return;
		}
		ComponentId componentId = entityComponents[componentTypeId];
		if(componentId == INVALID_ID) {
			spdlog::warn("Component already delete!");
			assert(false);
			return;
		}
		IComponent *component = getComponentPtr(componentId);
		assert(component != nullptr && "FATAL: Trying to remove a component which is not used by this entity!");
		getComponentContainer<T>()->destroyObject(component);
		unmapEntityComponent(entityId, componentId, componentTypeId);
		releaseComponentId(componentId);
	}

	template <class T>
	T *getComponent(EntityId entityId)
	{
		auto entityIdIndex = getIdIndex(entityId);
		if (entityIdIndex >= entityComponentMap_.size())
		{
			assert(false && "Entity has not any components");
			return;
		}

		auto &entityComponents = entityComponentMap_[entityIdIndex];
		ComponentTypeId componentTypeId = T::getTypeId();
		if (componentTypeId >= entityComponents.size())
		{
			assert(false && "Entity has not component");
			return;
		}
		ComponentId componentId = entityComponents[componentTypeId];
		if (componentId == INVALID_ID)
		{
			return nullptr;
		}
		return static_cast<T *>(componentIdManager_[componentId]);
	}

	void removeAllComponent(EntityId entityId)
	{
		auto entityIdIndex = getIdIndex(entityId);
		assert(entityIdIndex < entityComponentMap_.size() && "Entity has not any components");

		auto &entityComponents = entityComponentMap_[entityIdIndex];
		for (ComponentTypeId componentTypeId = 0; componentTypeId < entityComponents.size(); ++componentTypeId)
		{
			ComponentId componentId = entityComponents[componentTypeId];
			if (componentId == INVALID_ID)
			{
				continue;
			}
			IComponent *component = componentIdManager_[componentId];
			if (component == nullptr)
			{
				continue;
			}

			auto it = componentContainers_.find(componentTypeId);
			if (it != componentContainers_.end())
			{
				it->second->destroyObject(component);
			}
			else
			{
				assert(false && "Trying to release a component that wasn't created by ComponentManager!");
			}
			unmapEntityComponent(entityId, componentId, componentTypeId);
		}
	}

	template <class T>
	ComponentIterator<T> begin()
	{
		return getComponentContainer<T>()->begin();
	}

	template <class T>
	ComponentIterator<T> end()
	{
		return getComponentContainer<T>()->end();
	}

private:
	IComponent *getComponentPtr(ComponentId componentId)
	{
		return componentIdManager_[componentId];
	}

	void mapEntityComponent(EntityId entityId, ComponentId componentId, ComponentTypeId componentTypeId)
	{
		auto entityIdIndex = getIdIndex(entityId);
		if (entityComponentMap_.size() <= entityIdIndex)
		{
			entityComponentMap_.resize(entityIdIndex + TABLE_GROW);
		}
		auto &entityComponents = entityComponentMap_[entityIdIndex];
		auto componentsCount = TypeIdManager<IComponent>::getCount();

		if (entityComponents.size() < componentsCount)
		{
			entityComponents.resize(componentsCount, INVALID_ID);
		}
		assert(entityComponentMap_[entityIdIndex][componentTypeId] == INVALID_ID &&
			   "Entity already have this component");
		entityComponentMap_[entityIdIndex][componentTypeId] = componentId;
	}

	void unmapEntityComponent(EntityId entityId, ComponentId componentId, ComponentTypeId componentTypeId)
	{
		assert(entityComponentMap_[getIdIndex(entityId)][componentTypeId] == componentId &&
			   "FATAL: Entity Component ID mapping corruption!");
		entityComponentMap_[getIdIndex(entityId)][componentTypeId] = INVALID_ID;
	}

	template <typename T>
	ComponentContainer<T> *getComponentContainer()
	{
		auto componentTypeId = T::getTypeId();
		auto it = componentContainers_.find(componentTypeId);
		if (it == componentContainers_.end())
		{
			auto container = std::make_unique<ComponentContainer<T>>();
			auto ptr = container.get();
			componentContainers_[componentTypeId] = std::move(container);
			return ptr;
		}
		else
		{
			return static_cast<ComponentContainer<T> *>(it->second.get());
		}
	}

	ComponentId aqcuireComponentId(IComponent *component)
	{
		return componentIdManager_.acquiredId(component);
	}

	void releaseComponentId(ComponentId id)
	{
		componentIdManager_.releaseId(id);
	}

	ContainersMap componentContainers_;
	ObjectIdManager<IComponent> componentIdManager_;
	std::vector<std::vector<ComponentId>> entityComponentMap_;
};

#endif // VENGINE3D_COMPONENTMANAGER_H
