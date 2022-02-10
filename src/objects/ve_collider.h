//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_COLLIDER_H
#define VENGINE3D_VE_COLLIDER_H

#include "math/ve_vector.h"
#include "math/ve_transform.h"
#include "math/ve_matrix3.h"
#include "render/ve_attribute.h"
#include "render/ve_index_buffer.h"

#include <vector>
#include <memory>

namespace VE {
    enum class ColliderType {
        sphere = 0, box
    };

    class Collider {
    public:
        ColliderType shapeType() const { return shapeType_; }

        float mass() const { return mass_; }

        virtual void setLocalPosition(const Vector3 &localPosition) = 0;
        virtual Vector3 getCenterOfMass() const = 0;
        virtual Matrix3 getInertia() const = 0;
        virtual Vector3 farthestVertexInDirection(const Vector3 &direction) const = 0;
        virtual void setTransform(const Transform &transform) = 0;

        static std::vector<Vector3> getRenderNormals(const std::vector<Vector3>& vertex, const std::vector<unsigned int>& index){
            std::vector<Vector3> renderNormals;
            for (unsigned int i = 0; i < index.size(); i += 3) {
                Vector3 AB = vertex[index[i + 1]] - vertex[index[i + 0]];
                Vector3 BC = vertex[index[i + 2]] - vertex[index[i + 1]];

                renderNormals.push_back((AB * BC).getNormalized());
            }
            return renderNormals;
        }

        virtual ~Collider() {}

        Attribute<Vector3> vertexPosition;
        Attribute<Vector3> vertexNormals;
        IndexBuffer indexBuffer;
    protected:
        explicit Collider(ColliderType shapeType, float mass)
                : shapeType_(shapeType),
                  mass_(mass) {}

        float mass_;
        const ColliderType shapeType_;
    };

    using ColliderPtr = std::shared_ptr<Collider>;
    using ConstColliderPtr = std::shared_ptr<const Collider>;
    using CollidersPtrVector = std::vector<ColliderPtr>;
}


#endif //VENGINE3D_VE_COLLIDER_H
