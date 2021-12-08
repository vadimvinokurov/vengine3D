//
// Created by boris on 12/7/2021.
//

#include "ve_contact_points.h"
#include "ve_global_parameters.h"

using namespace VE;

inline void printFace(const ColliderFace &face, const Color &color = Color(1, 0, 0)) {
    face.vertices[0].drawPoint(12, color);
    face.vertices[1].drawPoint(12, color);
    face.vertices[2].drawPoint(12, color);
    face.vertices[3].drawPoint(12, color);
}


ContactPoint::ContactPoint(const BoxCollider &collider1, const BoxCollider &collider2, const Vector &contactNormal) : contactNormal_(contactNormal) {
    ColliderFace faceC1 = collider1.getFaceInDirection(contactNormal);
    ColliderFace faceC2 = collider2.getFaceInDirection(contactNormal * -1);

    bool isFaceC1Reference = selectReferenceEdge(faceC1, faceC2);
    if (isFaceC1Reference) {
        generateClipPlanes(faceC1);
        generateIncedentFaceVectors(faceC2);
    } else {
        generateClipPlanes(faceC2);
        generateIncedentFaceVectors(faceC1);
    }
}

bool ContactPoint::selectReferenceEdge(const ColliderFace &face1, const ColliderFace &face2) {
    return abs(face1.normal.dot(contactNormal_)) > abs(face2.normal.dot(contactNormal_));
}

void ContactPoint::get() {

    for (const auto &clipPlane: clipPlanes) {
        for(size_t i = 0; i < incedentFaceVectors.size(); i++){
            if(clipVector(incedentFaceVectors[i], clipPlane)){
                incedentFaceVectors[i] = incedentFaceVectors.back();
                incedentFaceVectors.pop_back();
                i--;
            }
        }
    }
    printIncVeretex();
}

void ContactPoint::generateIncedentFaceVectors(const ColliderFace &incedentFace) {
    for (unsigned int i = 0; i < incedentFace.vertices.size() - 1; i++) {
        incedentFaceVectors.emplace_back(incedentFace.vertices[i], incedentFace.vertices[i + 1]);
    }
    incedentFaceVectors.emplace_back(incedentFace.vertices.back(), incedentFace.vertices.front());
}

void ContactPoint::generateClipPlanes(const ColliderFace &referenceFace) {
    for (unsigned int i = 0; i < referenceFace.vertices.size() - 1; i++) {
        VE::Vector planeNormal = (referenceFace.vertices[i + 1] - referenceFace.vertices[i]) * referenceFace.normal;
        VE::Vector planeVertex = referenceFace.vertices[i];
        clipPlanes.emplace_back(planeNormal, planeVertex);
    }
    VE::Vector planeNormal = (referenceFace.vertices.front() - referenceFace.vertices.back()) * referenceFace.normal;
    VE::Vector planeVertex = referenceFace.vertices.back();
    clipPlanes.emplace_back(planeNormal, planeVertex);

    mainPlane = ClipPlane(referenceFace.normal, referenceFace.vertices.front());
}

bool ContactPoint::clipVector(std::pair<VE::Vector, VE::Vector> &vector, const ContactPoint::ClipPlane &clipPlane) {
    Vector &A = vector.first;
    Vector &B = vector.second;
    const Vector &n = clipPlane.normal;
    const Vector &P = clipPlane.point;
    float d = clipPlane.d;

    float aPosition = (A - P).dot(n);
    float bPosition = (B - P).dot(n);
    if (aPosition >= 0 && bPosition >= 0) {
        return true;
    } else if (aPosition < 0 && bPosition < 0) {
        return false;
    }
    float t = (d - A.dot(n)) / ((B - A).dot(n));
    Vector Q = A + (B - A) * t;
    if (aPosition >= 0) {
        vector.first = Q;
    } else {
        vector.second = Q;
    }
    return false;
}
