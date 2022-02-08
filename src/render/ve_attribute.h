//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_ATTRIBUTE_H
#define VENGINE3D_VE_ATTRIBUTE_H

#include "ve_stdlibs.h"

namespace VE {
    template<typename T>
    class Attribute {
    public:
        Attribute(const Attribute &other) = delete;
        Attribute &operator=(const Attribute &other) = delete;

        Attribute();
        ~Attribute();

        void setAttributePointer(unsigned int slot);

        void set(T *inputArray, unsigned int arrayLength);
        void set(std::vector<T> &input);

        void bindTo(unsigned int slot);
        void unBindFrom(unsigned int slot);

        unsigned int count();
        unsigned int getHandle();

    protected:
        unsigned handle_;
        unsigned count_;
    };
}



#endif //VENGINE3D_VE_ATTRIBUTE_H
