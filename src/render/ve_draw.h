//
// Created by boris on 2/7/2022.
//

#ifndef VENGINE3D_VE_DRAW_H
#define VENGINE3D_VE_DRAW_H

#include "ve_index_buffer.h"

namespace VE{
    enum class DrawMode {
        Points,
        LineStrip,
        LineLoop,
        Lines,
        Triangles,
        TriangleStrip,
        TriangleFan,
        Polygon
    };

    void draw(IndexBuffer& inIndexBuffer, DrawMode mode);
    void draw(unsigned int vertexCount, DrawMode mode);

    void drawInstance(IndexBuffer& inIndexBuffer, DrawMode mode, unsigned int instanceCount);
    void drawInstance(unsigned int vertexCount, DrawMode , unsigned int numInstance);
}


#endif //VENGINE3D_VE_DRAW_H
