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
        BoxCollider(float width = 1.0f, float height = 1.0f, float depth = 1.0f, float mass = 1.0f);

        virtual void setLocalTransform(const Transform &localTransform) override;
        virtual Vector farthestVertexInDirection(const Vector &direction) const override;
        virtual void setTransform(const Transform &transform) override;
        virtual const Vector getCenterOfMass() const override;

        ColliderFace getFaceInDirection(const Vector &direction) const;
        ColliderFace getFace(unsigned int faceNumber) const;

        virtual const void *verticesGLFormatData() const override;
        virtual const void *indicesGLFormatData(unsigned int offset = 0) const override;
        virtual unsigned int indecesSize() const override;
    private:
        void computeFaceNormals();
        void computeBoxInertia(float width, float height, float depth);
        void updateGlobalBuffer();

        std::array<Vector, 8> localVertices_;
        std::array<Vector, 8> globalVertices_;
        std::array<Vector, 6> localFaceNormals_;
        std::array<Vector, 6> globalFaceNormals_;
        const std::vector<unsigned int> indices_ = {3, 2, 1, 0,
                                                    4, 5, 6, 7,
                                                    0, 1, 5, 4,
                                                    2, 3, 7, 6,
                                                    3, 0, 4, 7,
                                                    1, 2, 6, 5};
    };
}


#endif //VENGINE3D_VE_BOX_COLLIDER_H
