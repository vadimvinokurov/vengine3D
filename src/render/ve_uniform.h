//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_UNIFORM_H
#define VENGINE3D_VE_UNIFORM_H

#include "glad/glad.h"
#include "ve_stdlibs.h"
#include "math/ve_quaternion.h"


namespace VE {
    template<typename T>
    class Uniform {
    public:
        Uniform() = delete;
        Uniform(const Uniform &) = delete;
        Uniform &operator=(const Uniform &) = delete;
        ~Uniform() = delete;

        static void set(unsigned int slot, const T &value);
        static void set(unsigned int slot, std::vector<T> &arr);
        static void set(unsigned int slot, T *arr, unsigned int arraySize);

    };

    template<typename T>
    void Uniform<T>::set(unsigned int slot, const T &value) {
        set(slot, static_cast<T *>(&value), 1);
    }

    template<typename T>
    void Uniform<T>::set(unsigned int slot, std::vector<T> &arr) {
        set(slot, arr.data(), static_cast<unsigned int>(arr.size()));
    }
}


#endif //VENGINE3D_VE_UNIFORM_H
