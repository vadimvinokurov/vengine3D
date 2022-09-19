//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_ICOMPONENT_H
#define VENGINE3D_ICOMPONENT_H

#include "VObject.h"
#include "Core/Utils/IdManagers.h"
#include "Core/Utils/TypeTraits.h"

#define GENERATE_COMPONENT_BODY(BasedClass)                                                                            \
public:                                                                                                                \
	using Super = BasedClass;                                                                                          \
	virtual ComponentTypeId getComponentTypeId() const                                                                 \
	{                                                                                                                  \
		return TypeIdManager<Component>::getId<std::remove_cv_t<std::remove_pointer_t<decltype(this)>>>();             \
	}                                                                                                                  \
	virtual ComponentTypeId getParentComponentTypeId() const                                                           \
	{                                                                                                                  \
		return TypeIdManager<Component>::getId<Super>();                                                               \
	}                                                                                                                  \
	static ComponentTypeId getTypeId()                                                                                 \
	{                                                                                                                  \
		return TypeIdManager<Component>::getId<get_class_type_by_method_t<decltype(&getComponentTypeId)>>();           \
	}                                                                                                                  \
                                                                                                                       \
private:

class Component : public VObject
{
	friend class ComponentManager;
	GENERATE_COMPONENT_BODY(VObject)

public:
	Component() = default;
	virtual ~Component() = default;

	ComponentId getComponentId() const
	{
		return id_;
	}

	EntityId getOwner() const
	{
		return owner_;
	}

	inline void SetActive(bool active)
	{
		active_ = active;
	}
	inline bool IsActive() const
	{
		return active_;
	}

protected:
	ComponentId hash_ = {};
	ComponentId id_ = INVALID_ID;
	EntityId owner_ = INVALID_ID;
	bool active_ = true;
};

#endif // VENGINE3D_ICOMPONENT_H
