//
// Created by boris on 7/7/2022.
//

#ifndef VENGINE3D_DELEGATE_TWOPARAM_H
#define VENGINE3D_DELEGATE_TWOPARAM_H

#include "Core/Objects/VObject.h"
#include <type_traits>

template <typename... Args>
struct IMethodWrapper : public VObjectDelegateMethod
{
	virtual void call(Args... args) = 0;
};

template <typename T, typename M, typename... Args>
class MethodWrapper : public IMethodWrapper<Args...>
{
public:
	MethodWrapper(T *object, M method) : object_(object), method_(method)
	{
	}

	void call(Args... args) override
	{
		if (!object_)
			return;
		(object_->*method_)(args...);
	};

	void clear() override
	{
		object_ = nullptr;
	};

private:
	T *object_;
	M method_;
};

template <typename... Args>
class Delegate
{
public:
	template <typename T, typename M>
	void connect(T *object, M method)
	{
		static_assert(std::is_base_of_v<VObject, T> && "Should connect only vobject");
		auto ptr = std::make_shared<MethodWrapper<T, M, Args...>>(object, method);
		methods_.push_back(ptr);
		object->onConnectToDelegate(ptr);
	};

	void call(Args... args)
	{
		for (auto &function : methods_)
		{
			function->call(args...);
		}
	}

private:
	std::vector<std::shared_ptr<IMethodWrapper<Args...>>> methods_;
};

#endif // VENGINE3D_DELEGATE_TWOPARAM_H
