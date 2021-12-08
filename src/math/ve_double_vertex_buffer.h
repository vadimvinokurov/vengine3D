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

        std::vector<VE::Vector>& buffer1(){return *pBuffer1;}
        std::vector<VE::Vector>& buffer2(){return *pBuffer2;}
    private:
        std::vector<VE::Vector> vertexBuffer1;
        std::vector<VE::Vector> vertexBuffer2;
        std::vector<VE::Vector>* pBuffer1;
        std::vector<VE::Vector>* pBuffer2;
    };
}

#endif //VENGINE3D_VE_DOUBLE_VERTEX_BUFFER_H
