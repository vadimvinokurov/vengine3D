//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_SHAPE_H
#define VENGINE3D_VE_SHAPE_H

#include "Math/ve_vector.h"
#include "Math/ve_transform.h"

#include <vector>
#include <memory>

namespace VE {
    class Shape {
    public:
        Shape();
        const float *verticesGLFormatData() const;
        const void *indicesGLFormatData(unsigned int offset = 0) const;
        virtual Vector farthestVertexInDirection(const Vector &direction) const = 0;

        virtual ~Shape();
    protected:
        std::vector<VE::Vector> vertices_;
        std::vector<unsigned int> indices_;
    };

    using ShapePtr = std::shared_ptr<Shape>;
}


#endif //VENGINE3D_VE_SHAPE_H
