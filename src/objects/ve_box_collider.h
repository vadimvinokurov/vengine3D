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
        BoxCollider(float width = 1.0f, float height = 1.0f, float depth = 1.0f, float mass = 1.0f, const Vector3 &localPosition = Vector3());

        void setLocalPosition(const Vector3 &localPosition) override;
        Vector3 farthestVertexInDirection(const Vector3 &direction) const override;
        void setTransform(const Transform &transform) override;
        Vector3 getCenterOfMass() const override;
        Matrix3 getInertia() const override;

        ColliderFace getFaceInDirection(const Vector3 &direction) const;
        ColliderFace getFace(unsigned int faceNumber) const;

    private:
        static std::vector<Vector3> computeVertices(float width, float height, float depth, const Vector3 &localPosition);
        static std::vector<Vector3> computeFaceNormals(const std::vector<Vector3> &vertices, const std::vector<unsigned int> &indices);
        static Vector3 computeCenterOfMass(const std::vector<Vector3> &vertices);


        const std::vector<unsigned int> indices_ = {3, 2, 1, 0,
                                                    4, 5, 6, 7,
                                                    0, 1, 5, 4,
                                                    2, 3, 7, 6,
                                                    3, 0, 4, 7,
                                                    1, 2, 6, 5};

        std::vector<Vector3> vertices_;
        std::vector<Vector3> faceNormals_;
        Vector3 centerOfMass_;
        float width_;
        float height_;
        float depth_;


        const std::vector<unsigned int> renderIndices_ = {0, 2, 1,
                                                          0, 3, 2,
                                                          0, 5, 4,
                                                          0, 1, 5,
                                                          2, 7, 6,
                                                          2, 3, 7,
                                                          5, 7, 4,
                                                          5, 6, 7,
                                                          1, 6, 5,
                                                          1, 2, 6,
                                                          3, 4, 7,
                                                          3, 0, 4};
    };
}


#endif //VENGINE3D_VE_BOX_COLLIDER_H
