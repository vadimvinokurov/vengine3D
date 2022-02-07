//
// Created by boris on 2/7/2022.
//
#include "glad/glad.h"
#include "ve_index_buffer.h"

VE::IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &handle_);
    count_ = 0;
}

VE::IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &handle_);
}

void VE::IndexBuffer::set(unsigned int *inputArray, unsigned int arrayLength) {
    count_ = arrayLength;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*count_, inputArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VE::IndexBuffer::set(std::vector<unsigned int> &input) {
    set(input.data(), input.size());
}


unsigned int VE::IndexBuffer::count() {
    return count_;
}

unsigned int VE::IndexBuffer::getHandle() {
    return handle_;
}
