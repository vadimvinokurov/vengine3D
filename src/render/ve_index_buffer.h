//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_INDEX_BUFFER_H
#define VENGINE3D_VE_INDEX_BUFFER_H

#include "ve_stdlibs.h"

namespace VE {
    class IndexBuffer {
    public:
        IndexBuffer(const IndexBuffer &other) = delete;
        IndexBuffer &operator=(const IndexBuffer &other) = delete;

        IndexBuffer();
        ~IndexBuffer();

        void set(const unsigned int *array, unsigned int arrayLength);
        void set(const std::vector<unsigned int> &input);

        unsigned int count();
        unsigned int getHandle();

    protected:
        unsigned handle_;
        unsigned count_;
    };
}


#endif //VENGINE3D_VE_INDEX_BUFFER_H
