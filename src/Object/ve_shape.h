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
    enum ShapeType {
        sphereShape = 0, boxShape, capsuleShape
    };

    class Shape {
    public:
        const float *verticesGLFormatData() const;
        const void *indicesGLFormatData(unsigned int offset = 0) const;

        virtual Vector farthestVertexInDirection(const Vector &direction) const = 0;
        ShapeType shapeType() const;
        virtual ~Shape();
    protected:
        explicit Shape(ShapeType shapeType);

        std::vector<VE::Vector> vertices_;
        std::vector<unsigned int> indices_;
        const ShapeType shapeType_;
    };

    using ShapePtr = std::shared_ptr<Shape>;

    struct GlobalShape {
        GlobalShape(ShapePtr s, VE::Transform t) : shapePtr(s), transform(t) {}

        ShapePtr shapePtr;
        VE::Transform transform;
    };
}


#endif //VENGINE3D_VE_SHAPE_H
