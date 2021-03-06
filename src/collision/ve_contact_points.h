//
// Created by boris on 12/7/2021.
//

#ifndef VENGINE3D_VE_CONTACT_POINTS_H
#define VENGINE3D_VE_CONTACT_POINTS_H

#include "objects/ve_colliders.h"
#include "math/ve_double_vertex_buffer.h"

namespace VE {
    inline std::vector<VE::Vector3>
    sphereSphereContactPoint(const SphereCollider &sphere1, const VE::SphereCollider &sphere2, const VE::Vector3 &contactNormal) {
        Vector3 centerToCenter = sphere2.getCenterOfMass() - sphere1.getCenterOfMass();
        return {sphere1.getCenterOfMass() + centerToCenter / 2};
    }

    inline std::vector<VE::Vector3>
    boxSphereContactPoint(const BoxCollider &box, const VE::SphereCollider &sphere, const VE::Vector3 &contactNormal) {
        return {sphere.getCenterOfMass() - contactNormal * sphere.getRadius()};
    }

    inline std::vector<VE::Vector3>
    boxSphereContactPoint(const VE::SphereCollider &sphere, const BoxCollider &box, const VE::Vector3 &contactNormal) {
        return {sphere.getCenterOfMass() + contactNormal * sphere.getRadius()};
    }

    class BoxBoxContactPoint {
    public:
        BoxBoxContactPoint(const VE::BoxCollider &collider1, const VE::BoxCollider &collider2, const VE::Vector3 &contactNormal);
        std::vector<VE::Vector3> get();
    private:
        struct ClipPlane {
            ClipPlane() {};

            ClipPlane(const VE::Vector3 &n, const VE::Vector3 &v) : normal(n), point(v) {
                d = point.dot(normal);
            };
            VE::Vector3 normal;
            VE::Vector3 point;
            float d;
        };

        bool selectReferenceEdge(const ColliderFace &face1, const ColliderFace &face2, const VE::Vector3 &contactNormal);
        void generateClipPlanes(const ColliderFace &referenceFace);
        bool vertexInsidePlane(const VE::Vector3 &A, const ClipPlane &clipPlane);
        VE::Vector3 intersectionPoint(const VE::Vector3 &A, const VE::Vector3 &B, const ClipPlane &clipPlane);
        void deleteVertexOutsideMainFace(std::vector<VE::Vector3> &vertices);

        std::vector<ClipPlane> clipPlanes;
        ClipPlane mainPlane;

        DoubleVertexBuffer doubleVertexBuffer;
    };

    inline std::vector<VE::Vector3> getContactsPoint(const Collider &collider1, const Collider &collider2, const VE::Vector3 &contactNormal) {
        struct IntersactionType {
            IntersactionType(ColliderType a, ColliderType b) : a_(a), b_(b) {}

            bool operator==(IntersactionType b) {
                return this->a_ == b.a_ && this->b_ == b.b_;
            }

        private:
            ColliderType a_;
            ColliderType b_;
        };

        const IntersactionType t[4] = {
                {ColliderType::box,    ColliderType::box},
                {ColliderType::box,    ColliderType::sphere},
                {ColliderType::sphere, ColliderType::box},
                {ColliderType::sphere, ColliderType::sphere},
        };

        IntersactionType intersactionType(collider1.shapeType(), collider2.shapeType());

        if (intersactionType == t[0]) {
            return BoxBoxContactPoint(static_cast<const BoxCollider &>(collider1), static_cast<const BoxCollider &>(collider2), contactNormal).get();
        } else if (intersactionType == t[1]) {
            return boxSphereContactPoint(static_cast<const BoxCollider &>(collider1), static_cast<const SphereCollider &>(collider2), contactNormal);
        } else if (intersactionType == t[2]) {
            return boxSphereContactPoint(static_cast<const SphereCollider &>(collider1), static_cast<const BoxCollider &>(collider2), contactNormal);
        } else if (intersactionType == t[3]) {
            return sphereSphereContactPoint(static_cast<const SphereCollider &>(collider1), static_cast<const SphereCollider &>(collider2),
                                            contactNormal);
        }
        assert(false && "getContactsPoint");
        return {};
    }
}


#endif //VENGINE3D_VE_CONTACT_POINTS_H
