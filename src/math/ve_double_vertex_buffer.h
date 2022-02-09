//
// Created by boris on 12/9/2021.
//

#ifndef VENGINE3D_VE_DOUBLE_VERTEX_BUFFER_H
#define VENGINE3D_VE_DOUBLE_VERTEX_BUFFER_H

#include <vector>
#include "ve_vector.h"

namespace VE{
    class DoubleVertexBuffer {
    public:
        DoubleVertexBuffer(){
            pBuffer1 = &vertexBuffer1;
            pBuffer2 = &vertexBuffer2;
        };
        void swapBuffer(){
            std::swap(pBuffer1, pBuffer2);
        };

        std::vector<VE::Vector3>& buffer1(){return *pBuffer1;}
        std::vector<VE::Vector3>& buffer2(){return *pBuffer2;}
    private:
        std::vector<VE::Vector3> vertexBuffer1;
        std::vector<VE::Vector3> vertexBuffer2;
        std::vector<VE::Vector3>* pBuffer1;
        std::vector<VE::Vector3>* pBuffer2;
    };
}

#endif //VENGINE3D_VE_DOUBLE_VERTEX_BUFFER_H
