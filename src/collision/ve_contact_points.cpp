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
        incidentFaceVertices = faceC2.vertices;
    } else {
        generateClipPlanes(faceC2);
        incidentFaceVertices = faceC1.vertices;
    }
}

bool ContactPoint::selectReferenceEdge(const ColliderFace &face1, const ColliderFace &face2) {
    return abs(face1.normal.dot(contactNormal_)) > abs(face2.normal.dot(contactNormal_));
}

std::vector<VE::Vector> ContactPoint::get() {
    std::vector<VE::Vector> outputVertices = incidentFaceVertices;

    for (const ClipPlane &clipPlane: clipPlanes) {
        incidentFaceVertices = outputVertices;
        outputVertices.clear();

        for (size_t i = 0; i < incidentFaceVertices.size(); i++) {
            const VE::Vector &B = incidentFaceVertices[i];
            const VE::Vector &A = incidentFaceVertices[(i - 1) % incidentFaceVertices.size()];
            assert(!((i == 0) && ((i - 1) % incidentFaceVertices.size() != incidentFaceVertices.size() - 1)));

            if(vertexInsidePlane(B, clipPlane)) {
                if(!vertexInsidePlane(A, clipPlane)) {
                    outputVertices.emplace_back(intersectionPoint(A,B,clipPlane));
                }
                outputVertices.emplace_back(B);
            } else if (vertexInsidePlane(A, clipPlane)){
                outputVertices.emplace_back(intersectionPoint(A,B,clipPlane));
            }
        }
    }
    incidentFaceVertices = outputVertices;
    printIncVeretices();
    return incidentFaceVertices;
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

VE::Vector ContactPoint::intersectionPoint(const VE::Vector &A, const VE::Vector &B, const ClipPlane &clipPlane) {
    const Vector &n = clipPlane.normal;
    const Vector &P = clipPlane.point;
    float d = clipPlane.d;
    float t = (d - A.dot(n)) / ((B - A).dot(n));
    return A + (B - A) * t;
}

void ContactPoint::printIncVeretices() {
    for (const auto &vertex: incidentFaceVertices) {
        vertex.drawPoint(12, Color(1, 0, 0));
    }
}

bool ContactPoint::vertexInsidePlane(const Vector &A, const ContactPoint::ClipPlane &clipPlane) {
    return (A - clipPlane.point).dot(clipPlane.normal) <= 0;
}
