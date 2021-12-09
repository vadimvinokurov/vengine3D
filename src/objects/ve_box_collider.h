//
// Created by boris on 11/24/2021.
//

#ifndef VENGINE3D_VE_BOX_COLLIDER_H
#define VENGINE3D_VE_BOX_COLLIDER_H

#include "ve_collider.h"
#include "math/ve_transform.h"
#include "ve_face.h"

namespace VE {
    class BoxCollider : public Collider {
    public:
        BoxCollider(float width = 1.0f, float height = 1.0f, float depth = 1.0f, float mass = 1.0f,  const Transform &localTransform = Transform());
        BoxCollider(float width, float height, float depth, const Transform &localTransform);
        BoxCollider(float mass,  const Transform &localTransform = Transform());
        BoxCollider(const Transform &localTransform);

        virtual Vector farthestVertexInDirection(const Vector &direction) const override;
        virtual void setTransform(const Transform &transform) override;

        ColliderFace getFaceInDirection(const Vector &direction) const;
        ColliderFace getFace(unsigned int faceNumber) const;
    private:
        void computeVertices(float width, float height, float depth, const Transform &localTransform);
        void computeFaceNormals();
        void computeCenterOfMass();
        void computeBoxInertia(float width, float height, float depth);
        void setGlvertices();
        void initGlobalBuffer();

        const std::vector<unsigned int> indices_;
        std::vector<Vector> localFaceNormals_;
        std::vector<Vector> globalFaceNormals_;
        std::vector<Vector> localVertices_;
        std::vector<Vector> globalVertices_;
    };
}


#endif //VENGINE3D_VE_BOX_COLLIDER_H
