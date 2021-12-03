//
// Created by boris on 11/24/2021.
//

#ifndef VENGINE3D_VE_BOX_COLLIDER_H
#define VENGINE3D_VE_BOX_COLLIDER_H

#include "ve_collider.h"
#include "Math/ve_transform.h"

namespace VE {
    class BoxCollider : public Collider {
    public:
        BoxCollider(float width = 1.0f, float height = 1.0f, float depth = 1.0f, Transform localTransform = Transform());
        BoxCollider(Transform localTransform);
        virtual Vector farthestVertexInDirection(const Vector &direction) const override;
        virtual void setTransform(const Transform &transform) override;
    private:
        void setGlvertices();
        void computeCenterOfMass();
        std::vector<Vector> localVertices_;
        std::vector<Vector> globalVertices_;
    };
}


#endif //VENGINE3D_VE_BOX_COLLIDER_H
