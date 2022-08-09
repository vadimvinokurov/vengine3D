//
// Created by boris on 8/9/2022.
//

#ifndef VENGINE3D_ISYSTEM_H
#define VENGINE3D_ISYSTEM_H

#include "VObject.h"
#include "ECS/Utils/IdManagers.h"

class ISystem : public VObject
{
	friend class SystemManager;
public:
	ISystem(float updateInterval = -1.0f) : updateInterval_(updateInterval){};
	virtual ~ISystem() = default;

	virtual void preUpdate(float dt){};
	virtual void update(float dt){};
	virtual void postUpdate(float dt){};

	virtual SystemTypeId getSystemTypeId() const = 0;

private:
	float timeSinceLastUpdate_ = 0;
	float updateInterval_ = -1.0f;
	bool enabled = true;
	bool needsUpdate = true;
	SystemManager * systemManager_;
};

#endif // VENGINE3D_ISYSTEM_H
