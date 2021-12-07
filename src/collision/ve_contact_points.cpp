//
// Created by boris on 12/7/2021.
//

#include "ve_contact_points.h"

using namespace VE;

inline void printFace(const ColliderFace &face, const Color &color = Color(1, 0, 0)) {
    face.vertices[0].drawPoint(12, color);
    face.vertices[1].drawPoint(12, color);
    face.vertices[2].drawPoint(12, color);
    face.vertices[3].drawPoint(12, color);
}

bool selectReferenceEdge(const ColliderFace &face1, const ColliderFace &face2, const VE::Vector &contactNormal) {
    return abs(face1.normal.dot(contactNormal)) > abs(face2.normal.dot(contactNormal));
}


void VE::getContactPoints(const VE::BoxCollider &collider1, const VE::BoxCollider &collider2, const VE::Vector &contactNormal) {
    ColliderFace faceC1 = collider1.getFaceInDirection(contactNormal);
    ColliderFace faceC2 = collider2.getFaceInDirection(contactNormal * -1);

    bool isFaceC1Reference = selectReferenceEdge(faceC1, faceC2, contactNormal);
    ColliderFace &refFace = isFaceC1Reference ? faceC1 : faceC2;
    ColliderFace &incFace = isFaceC1Reference ? faceC2 : faceC1;
    incFace.projectionOn(refFace);


    printFace(incFace);


}

