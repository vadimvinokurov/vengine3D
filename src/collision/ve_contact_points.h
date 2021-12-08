//
// Created by boris on 12/7/2021.
//

#ifndef VENGINE3D_VE_CONTACT_POINTS_H
#define VENGINE3D_VE_CONTACT_POINTS_H

#include "objects/ve_box_collider.h"

namespace VE {
    void getContactPoints(const BoxCollider &collider1, const BoxCollider &collider2, const Vector &contactNormal);

    class ContactPoint {
    public:
        ContactPoint(const VE::BoxCollider &collider1, const VE::BoxCollider &collider2, const VE::Vector &contactNormal);
        void get();
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

        bool selectReferenceEdge(const ColliderFace &face1, const ColliderFace &face2);
        void generateIncedentFaceVectors(const ColliderFace &incedentFace);
        void generateClipPlanes(const ColliderFace &referenceFace);

        bool clipVector(std::pair<VE::Vector, VE::Vector> &vector, const ClipPlane &clipPlane);


        void printIncVeretex() {
            for (const auto &vector: incedentFaceVectors) {
                vector.first.drawPoint(12, Color(1, 0, 0));
                vector.second.drawPoint(12, Color(0, 1, 0));
            }
        }


        VE::Vector contactNormal_;
        std::vector<ClipPlane> clipPlanes;
        ClipPlane mainPlane;

        std::vector<std::pair<VE::Vector, VE::Vector>> incedentFaceVectors;
    };
}


#endif //VENGINE3D_VE_CONTACT_POINTS_H
