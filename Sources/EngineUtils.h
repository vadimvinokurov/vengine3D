//
// Created by boris on 7/10/2022.
//

#ifndef VENGINE3D_ENGINEUTILS_H
#define VENGINE3D_ENGINEUTILS_H

#include <memory>

template<typename T, typename... Args>
std::shared_ptr<T> CreateObject(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
std::unique_ptr<T> CreateUniqueObject(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

#endif	//VENGINE3D_ENGINEUTILS_H
