//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_ATTRIBUTE_H
#define VENGINE3D_VE_ATTRIBUTE_H

#include <glad/glad.h>

#include "stdlibs.h"
#include "math/ve_vector.h"
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
    private:

    };
}

template<typename T>
VE::Attribute<T>::Attribute() {
    glGenBuffers(1, &handle_);
    count_ = 0;
}

template<typename T>
VE::Attribute<T>::~Attribute() {
    glDeleteBuffers(1, &handle_);
}

template<typename T>
void VE::Attribute<T>::set(T *inputArray, unsigned int arrayLength) {
    count_ = arrayLength;
    unsigned int size = sizeof(T);

    glBindBuffer(GL_ARRAY_BUFFER, handle_);
    glBufferData(GL_ARRAY_BUFFER, size * count_, inputArray, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void VE::Attribute<T>::set(std::vector<T> &input) {
    set(input.data(), input.size());
}

template<typename T>
void VE::Attribute<T>::bindTo(unsigned int slot) {
    glBindBuffer(GL_ARRAY_BUFFER, handle_);
    glEnableVertexAttribArray(slot);
    setAttributePointer(slot);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void VE::Attribute<T>::unBindFrom(unsigned int slot) {
    glBindBuffer(GL_ARRAY_BUFFER, handle_);
    glEnableVertexAttribArray(slot);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
unsigned int VE::Attribute<T>::count() {
    return count_;
}

template<typename T>
unsigned int VE::Attribute<T>::getHandle() {
    return handle_;
}


#endif //VENGINE3D_VE_ATTRIBUTE_H
