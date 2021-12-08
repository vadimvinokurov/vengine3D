//
// Created by boris on 12/7/2021.
//

#ifndef VENGINE3D_VE_CONTACT_POINTS_H
#define VENGINE3D_VE_CONTACT_POINTS_H

#include "objects/ve_box_collider.h"
#include "math/ve_double_vertex_buffer.h"

namespace VE {
    void getContactPoints(const BoxCollider &collider1, const BoxCollider &collider2, const Vector &contactNormal);

    class ContactPoint {
    public:
        ContactPoint(const VE::BoxCollider &collider1, const VE::BoxCollider &collider2, const VE::Vector &contactNormal);
        std::vector<VE::Vector> get();
    private:
        struct ClipPlane {
            ClipPlane() {};

            ClipPlane(const VE::Vector &n, const VE::Vector &v) : normal(n), point(v) {
                d = point.dot(normal);
            };
            VE::Vector normal;
            VE::Vector point;
            float d;
        };

        bool selectReferenceEdge(const ColliderFace &face1, const ColliderFace &face2, const VE::Vector& contactNormal);
        void generateClipPlanes(const ColliderFace &referenceFace);
        bool vertexInsidePlane(const VE::Vector &A, const ClipPlane &clipPlane);
        VE::Vector intersectionPoint(const VE::Vector &A, const VE::Vector &B, const ClipPlane &clipPlane);
        void deleteVertexOutsideMainFace(std::vector<VE::Vector>& vertices);

        std::vector<ClipPlane> clipPlanes;
        ClipPlane mainPlane;

        DoubleVertexBuffer doubleVertexBuffer;
    };
}


#endif //VENGINE3D_VE_CONTACT_POINTS_H
