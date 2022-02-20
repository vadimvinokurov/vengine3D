//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_ATTRIBUTE_H
#define VENGINE3D_VE_ATTRIBUTE_H

#include "ve_core.h"

namespace VE {
    template<typename T>
    class Attribute {
    public:
        Attribute(const Attribute &other) = delete;
        Attribute &operator=(const Attribute &other) = delete;

        Attribute();
        ~Attribute();

        void set(const T *inputArray, unsigned int arrayLength);
        void set(const std::vector<T> &input);

        void bindTo(unsigned int slot);
        void unBindFrom(unsigned int slot);

        unsigned int count();
        unsigned int getHandle();
    private:
        void setAttributePointer(unsigned int slot);

    protected:
        unsigned int handle_;
        unsigned int count_;
    };
}



#endif //VENGINE3D_VE_ATTRIBUTE_H
