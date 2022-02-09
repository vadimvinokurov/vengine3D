//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_UNIFORM_H
#define VENGINE3D_VE_UNIFORM_H


#include "ve_stdlibs.h"



namespace VE {
    template<typename T>
    class Uniform {
    public:
        Uniform() = delete;
        Uniform(const Uniform &) = delete;
        Uniform &operator=(const Uniform &) = delete;
        ~Uniform() = delete;

        static void set(unsigned int slot, const T &value);
        static void set(unsigned int slot, const std::vector<T> &arr);
        static void set(unsigned int slot, const T *arr, unsigned int arraySize);

    };
}


#endif //VENGINE3D_VE_UNIFORM_H
