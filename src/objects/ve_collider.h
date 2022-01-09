//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_COLLIDER_H
#define VENGINE3D_VE_COLLIDER_H

#include "math/ve_vector.h"
#include "math/ve_transform.h"
#include "math/ve_matrix33.h"

#include <vector>
#include <memory>

namespace VE {
    enum class ColliderType {
        sphere = 0, box, capsule
    };

    class Collider {
    public:
        ColliderType shapeType() const { return shapeType_; }

        float mass() const { return mass_; }

        virtual Vector getCenterOfMass() const = 0;
        virtual Matrix33 getInertia() const = 0;
        virtual void setLocalTransform(const Transform &localTransform) = 0;
        virtual Vector farthestVertexInDirection(const Vector &direction) const = 0;
        virtual void setTransform(const Transform &transform) = 0;

        virtual const void *verticesGLFormatData() const = 0;
        virtual const void *indicesGLFormatData(unsigned int offset = 0) const = 0;
        virtual unsigned int indecesSize() const = 0;

        virtual ~Collider() {}

    protected:
        explicit Collider(ColliderType shapeType, float mass)
                : shapeType_(shapeType),
                  mass_(mass) {}

        float mass_;
        const ColliderType shapeType_;
    };

    using ColliderPtr = std::shared_ptr<Collider>;
    using ConstColliderPtr = std::shared_ptr<const Collider>;
}


#endif //VENGINE3D_VE_COLLIDER_H
