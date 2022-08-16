//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_VOBJECT_H
#define VENGINE3D_VOBJECT_H

#include "spdlog/spdlog.h"
#include <list>

struct VObjectDelegateMethod
{
	virtual void clear() = 0;
};

struct VObject
{
	VObject() = default;
	virtual ~VObject()
	{
		for (auto &m : methodList)
		{
			m->clear();
		}
	};
	void onConnectToDelegate(std::shared_ptr<VObjectDelegateMethod> delegateMethod)
	{
		spdlog::warn("Connect to delegate");
		methodList.push_back(delegateMethod);
	}

private:
	std::list<std::shared_ptr<VObjectDelegateMethod>> methodList;
};

#endif // VENGINE3D_VOBJECT_H
