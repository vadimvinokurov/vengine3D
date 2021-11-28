//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_COLLIDER_H
#define VENGINE3D_VE_COLLIDER_H

#include "Math/ve_vector.h"
#include "Math/ve_transform.h"

#include <vector>
#include <memory>

namespace VE {
    enum class ColliderType {
        sphere = 0, box, capsule
    };

    class Collider {
    public:
        const float *verticesGLFormatData() const;
        const void *indicesGLFormatData(unsigned int offset = 0) const;

        virtual Vector farthestVertexInDirection(const Vector &direction) const = 0;
        ColliderType shapeType() const;
        unsigned int indecesSize() const;

        virtual void setTransform(const Transform &transform);

        void setColor(const Color& color) const;
        Color color() const;

        virtual ~Collider();
    protected:
        explicit Collider(ColliderType shapeType);


        const ColliderType shapeType_;
        Vector localCenterOfMass_;
        Vector globalCenterOfMass_;


        mutable VE::Color color_;
        std::vector<Vector> glvertices_;
        std::vector<unsigned int> glindices_;
    };

    using ColliderPtr = std::shared_ptr<Collider>;
    using ConstColliderPtr = std::shared_ptr<const Collider>;
}


#endif //VENGINE3D_VE_COLLIDER_H
