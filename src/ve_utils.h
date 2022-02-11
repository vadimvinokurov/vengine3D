//
// Created by boris on 2/12/2022.
//

#ifndef VENGINE3D_VE_UTILS_H
#define VENGINE3D_VE_UTILS_H

template<typename T, typename ...Args>
std::shared_ptr<T> create(Args... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif //VENGINE3D_VE_UTILS_H
