//
// Created by boris on 7/7/2022.
//

#ifndef VENGINE3D_DELEGATE_TWOPARAM_H
#define VENGINE3D_DELEGATE_TWOPARAM_H

template<typename... Args>
class IMethodWrapper {
public:
	virtual void call(Args... args) = 0;
};

template<typename T, typename M, typename... Args>
class MethodWrapper : public IMethodWrapper<Args...> {
public:
	MethodWrapper(T* object, M method) : object_(object), method_(method) {}

	void call(Args... args) override {
		(object_->*method_)(args...);
	};

private:
	T* object_;
	M method_;
};

template<typename... Args>
class Delegate {
public:
	template<typename T, typename M>
	void connect(T* object, M method) {
		auto uniqptr = std::make_shared<MethodWrapper<T, M, Args...>>(object, method);
		methods_.push_back(uniqptr);
	};

	void connect(std::function<void(Args...)> functor) {
		functors_.push_back(functor);
	};

	void call(Args... args) {
		for (auto& function : methods_) { function->call(args...); }
		for (auto& function : functors_) { function(args...); }
	}

private:
	std::vector<std::shared_ptr<IMethodWrapper<Args...>>> methods_;
	std::vector<std::function<void(Args...)>> functors_;
};


#endif	//VENGINE3D_DELEGATE_TWOPARAM_H
